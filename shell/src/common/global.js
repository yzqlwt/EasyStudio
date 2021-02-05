const { remote } = window.require('electron');
const { join } = window.require('path');
const fs = window.require('fs');

function fileExists(filePath)
{
    try
    {
        return fs.statSync(filePath).isFile();
    }
    catch (err)
    {
        return false;
    }
}

export const getResourcePath = function(){
    let shareObject = remote.getGlobal('shareObject')
    return shareObject.resourcePath;
}

export const getIconPath = function(name){
    let path = getResourcePath();
    console.log(path, name)
    return join(path, 'assets', 'images', name);
}

export const getServerPath = function(){
    let path = getResourcePath();
    return join(path, 'assets', 'kernel', 'EasyStudio.exe');
}

export const getConfigPath = function(){
    const USER_HOME = process.env.HOME || process.env.USERPROFILE
    return join(USER_HOME, 'AppData', 'Local', 'EasyStudio', 'config.json')
}

export const getTokenPath = function(){
    const USER_HOME = process.env.HOME || process.env.USERPROFILE
    return join(USER_HOME, 'AppData', 'Local', 'EasyStudio', 'token.json')
}

export const getConfig = function(){
    const file = getConfigPath()
    if (fileExists(file)){
        const content = fs.readFileSync(file)
        return JSON.parse(content)
    }
    return {}
}

export const getCCSPath = function(){
    const config = getConfig();
    return config.CCSPath || "undefined";
}

export const setCCSPath = function(value){
    const file = getConfigPath()
    let config = {}
    if (fileExists(file)){
        config = JSON.parse(fs.readFileSync(file))
    }
    config.CCSPath = value;
    fs.writeFileSync(file, JSON.stringify(config))
}

export const setToken = function(token){
    const file = getTokenPath();
    fs.writeFileSync(file, token);
}

export const getToken = function(){
    const file = getTokenPath();
    if (fileExists(file)){
        return JSON.parse(fs.readFileSync(file))
    }
    return null
}
