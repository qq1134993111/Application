#pragma once

//#include "iguana/reflection.hpp"
#include <iguana/xml_reader.hpp>
#include <iguana/xml_writer.hpp>
//#include <iguana/json_reader.hpp>
//#include <iguana/json_writer.hpp>


/*
    ConfigData data;
    data.app_name = GetAppLicationName();
    data.id = 100;
    //data.plugins = {PluginInfo{"name1", GetExeFilePath(), 1}, PluginInfo{"name2", GetExeFilePath(), 2}};
    iguana::xml_attr_t<PluginInfo> a1;
    a1.value() = PluginInfo{"name1", GetExeFilePath(), 1};
    a1.attr()["id"] = "1";
    iguana::xml_attr_t<PluginInfo> a2;
    a2.value() = PluginInfo{"name1", GetExeFilePath(), 2};
    a2.attr()["id"] = "2";
    data.plugins.plugins.push_back(a1);
    data.plugins.plugins.push_back(a2);

    std::string str;
    iguana::to_xml<true>(data, str);
*/
/*

<root>
    <id>100</id>
    <name>Application</name>
    <plugins>
        <row id="1">
            <plugin_name>name1</plugin_name>
            <plugin_path>D:\DEV\Application\x64\Debug</plugin_path>
            <version>1</version>
        </row>
        <row id="2">
            <plugin_name>name1</plugin_name>
            <plugin_path>D:\DEV\Application\x64\Debug</plugin_path>
            <version>2</version>
        </row>
    </plugins>
</root>


*/
struct PluginInfo
{
    std::string name;
    std::string path;
    int32_t version;
};
// REFLECTION(PluginInfo, name, path, version);
REFLECTION_ALIAS(PluginInfo, "PluginInfo", FLDALIAS(&PluginInfo::name, "plugin_name"),
                 FLDALIAS(&PluginInfo::path, "plugin_path"), FLDALIAS(&PluginInfo::version, "version"));

struct PluginInfos
{
    std::vector<iguana::xml_attr_t<PluginInfo>> plugins;
};
REFLECTION_ALIAS(PluginInfos, "PluginInfos", FLDALIAS(&PluginInfos::plugins, "row"));

struct ConfigData
{
    int32_t id;
    std::string app_name;
    PluginInfos plugins;
};
// REFLECTION(ConfigData, id, app_name, plugins);
REFLECTION_ALIAS(ConfigData, "root", FLDALIAS(&ConfigData::id, "id"), FLDALIAS(&ConfigData::app_name, "name"),
                 FLDALIAS(&ConfigData::plugins, "plugins"));
