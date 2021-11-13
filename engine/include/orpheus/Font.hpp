#pragma once

#include <string>
#include <unordered_map>

namespace orpheus {
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
            std::uint64_t m_id;
            float         m_advance;
            Rect          m_worldRect;
            Rect          m_atlasRect;

        public:
            Glyph() : m_id(0), m_advance(0) {}

            Glyph(std::uint64_t id, float advance, Rect worldRect, Rect atlasRect) :
                m_id(id),
                m_advance(advance),
                m_worldRect(worldRect),
                m_atlasRect(atlasRect)
            { }

            std::uint64_t getID() const {
                return m_id;
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
        std::string m_name;
        std::unordered_map<std::uint64_t, Glyph> m_glyphs;
        float m_distanceRange;
        float m_descender;
        float m_lineHeight;

    public:
        Font(const std::string& name);

        const std::string& getName() const;
        const Glyph& getGlyph(std::uint64_t id) const;
        float getDistanceRange() const;
        float getDescender() const;
    };
}
