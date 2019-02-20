#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings
{
    static Settings& instance()
    {
        static Settings self;
        return self;
    }

    std::size_t pixmapSize  = 30;

    std::size_t levelHeight = 15;
    std::size_t levelWidth  = 30;

    QString assetsPath_;
};

#endif // SETTINGS_H
