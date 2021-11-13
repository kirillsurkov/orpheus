#include "orpheus/Font.hpp"

#include </home/nio_x/work/cpp/orpheus-build-windows/game/thirdparty/nlohmann/single_include/nlohmann/json.hpp>//nlohmann/json.hpp>

#include <fstream>

namespace orpheus {
    Font::Font(const std::string& name) :
        m_name(name)
    {
        std::ifstream input("./res/fonts/" + name + "/layout.json");
        nlohmann::json json;
        input >> json;

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
                atlasOffset.width = (static_cast<float>(atlasIt->at("right")) - atlasOffset.x);
                atlasOffset.height = (static_cast<float>(atlasIt->at("top")) - atlasOffset.y);
            }

            m_glyphs.try_emplace(glyph["unicode"], glyph["unicode"], glyph["advance"], worldOffset, atlasOffset);
        }

        m_distanceRange = json["atlas"]["distanceRange"];
        m_descender = json["metrics"]["descender"];
        m_lineHeight = json["metrics"]["lineHeight"];
    }

    const std::string& Font::getName() const {
        return m_name;
    }

    const Font::Glyph& Font::getGlyph(std::uint64_t id) const  {
        auto it = m_glyphs.find(id);
        if (it == m_glyphs.end()) {
            throw std::runtime_error("No glyph with id " + std::to_string(id) + " in font " + m_name);
        }
        return it->second;
    }

    float Font::getDistanceRange() const {
        return m_distanceRange;
    }

    float Font::getDescender() const {
        return m_descender;
    }
}
