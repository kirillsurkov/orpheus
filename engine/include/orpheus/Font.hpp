#pragma once

#include "orpheus/Texture/Texture.hpp"

#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace Orpheus {
    class Font {
    public:
        class Glyph {
        public:
            struct Rect {
                float x = 0;
                float y = 0;
                float width = 0;
                float height = 0;
            };

        private:
            float m_advance;
            Rect m_worldRect;
            Rect m_atlasRect;

        public:
            Glyph(float advance, Rect worldRect, Rect atlasRect) :
                m_advance(advance),
                m_worldRect(worldRect),
                m_atlasRect(atlasRect)
            {
            }

            float getAdvance() const {
                return m_advance;
            }

            const Rect& getWorldRect() const {
                return m_worldRect;
            }

            const Rect& getAtlasRect() const {
                return m_atlasRect;
            }
        };

    private:
        Texture::Texture m_atlas;
        std::unordered_map<std::size_t, Glyph> m_glyphs;
        float m_distanceRange;
        float m_descender;
        float m_lineHeight;

        Font(const Texture::Texture& atlas, const std::unordered_map<std::size_t, Glyph> glyphs, float distanceRange, float descender, float lineHeight) :
            m_atlas(atlas),
            m_glyphs(glyphs),
            m_distanceRange(distanceRange),
            m_descender(descender),
            m_lineHeight(lineHeight)
        {
        }

    public:
        static Font load(const std::string& name) {
            std::string basePath = "./res/fonts/" + name;
            auto atlas = Texture::Texture::loadPNG(basePath + "/atlas.png");
            std::size_t atlasWidth = atlas.getWidth();
            std::size_t atlasHeight = atlas.getHeight();

            std::ifstream input(basePath + "/layout.json");
            nlohmann::json json;
            input >> json;

            std::unordered_map<std::size_t, Glyph> glyphs;

            for (const auto& glyph : json["glyphs"]) {
                Glyph::Rect worldOffset;
                auto worldIt = glyph.find("planeBounds");
                if (worldIt != glyph.end()) {
                    worldOffset.x = worldIt->at("left");
                    worldOffset.y = worldIt->at("bottom");
                    worldOffset.width = static_cast<float>(worldIt->at("right")) - worldOffset.x;
                    worldOffset.height = static_cast<float>(worldIt->at("top")) - worldOffset.y;
                }

                Glyph::Rect atlasOffset;
                auto atlasIt = glyph.find("atlasBounds");
                if (atlasIt != glyph.end()) {
                    atlasOffset.x = atlasIt->at("left");
                    atlasOffset.y = atlasIt->at("bottom");
                    atlasOffset.width = (static_cast<float>(atlasIt->at("right")) - atlasOffset.x) / atlasWidth;
                    atlasOffset.height = (static_cast<float>(atlasIt->at("top")) - atlasOffset.y) / atlasHeight;
                    atlasOffset.x /= atlasWidth;
                    atlasOffset.y /= atlasHeight;
                }

                glyphs.try_emplace(glyph["unicode"], glyph["advance"], worldOffset, atlasOffset);
            }

            return Font(atlas, glyphs, json["atlas"]["distanceRange"], json["metrics"]["descender"], json["metrics"]["lineHeight"]);
        }

        const Texture::Texture& getAtlas() const {
            return m_atlas;
        }

        const std::unordered_map<std::size_t, Glyph>& getGlyphs() const {
            return m_glyphs;
        }

        float getDistanceRange() const {
            return m_distanceRange;
        }

        float getDescender() const {
            return m_descender;
        }

        float getLineHeight() const {
            return m_lineHeight;
        }
    };
}
