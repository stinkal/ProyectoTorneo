#include "persistencia.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

namespace persistencia {

static std::string escapeString(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '"') out += "\\\"";
        else out += c;
    }
    return out;
}

bool guardarEquiposJSON(const std::vector<Equipo*>& equipos, const std::string& filepath) {
    std::ofstream ofs(filepath, std::ofstream::out | std::ofstream::trunc);
    if (!ofs.is_open()) return false;

    ofs << "[";
    bool first = true;
    for (auto e : equipos) {
        if (!first) ofs << ",";
        ofs << "\"" << escapeString(e->getNombre()) << "\"";
        first = false;
    }
    ofs << "]\n";
    ofs.close();
    return true;
}

bool cargarEquiposJSON(std::vector<Equipo*>& outEquipos, const std::string& filepath) {
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) return false;

    std::ostringstream ss;
    ss << ifs.rdbuf();
    std::string s = ss.str();
    ifs.close();

    outEquipos.clear();

    size_t i = 0;
    auto skip_ws = [&]() {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    };

    skip_ws();
    if (i >= s.size() || s[i] != '[') return false;
    ++i;
    skip_ws();

    while (i < s.size()) {
        skip_ws();
        if (i < s.size() && s[i] == ']') { ++i; break; }
        if (i >= s.size() || s[i] != '"') return false;
        ++i;
        std::string cur;
        while (i < s.size()) {
            char c = s[i++];
            if (c == '\\') {
                if (i >= s.size()) return false;
                char esc = s[i++];
                if (esc == '\\') cur += '\\';
                else if (esc == '"') cur += '"';
                else {
                    cur += esc;
                }
            } else if (c == '"') {
                break;
            } else {
                cur += c;
            }
        }
        Equipo* e = new Equipo(cur);
        outEquipos.push_back(e);

        skip_ws();
        if (i < s.size() && s[i] == ',') {
            ++i;
            skip_ws();
            continue;
        } else if (i < s.size() && s[i] == ']') {
            ++i;
            break;
        } else {
            skip_ws();
            if (i >= s.size()) break;
        }
    }

    return true;
}

}