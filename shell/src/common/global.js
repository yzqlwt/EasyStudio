const { remote } = window.require('electron');
const { join } = window.require('path');
export const getResourcePath = function(){
    let shareObject = remote.getGlobal('shareObject')
    return shareObject.resourcePath;
}

export const getServerPath = function(){
    let path = getResourcePath();
    return join(path, 'assets', 'kernel', 'EasyStudio.exe');
}
