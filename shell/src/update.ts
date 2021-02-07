import { dialog } from 'electron'
import { autoUpdater } from 'electron-updater'

autoUpdater.autoDownload = false

autoUpdater.on('error', (error) => {
  dialog.showErrorBox('Error: ', error == null ? "unknown" : (error.stack || error).toString())
})

autoUpdater.on('update-available', (info) => {
  dialog.showMessageBox({
    type: 'info',
    title: '更新提示',
    message: '发现有新版本'+ info.version +'，是否更新？',
    buttons: ['更新', '不了'],
    cancelId: 1,
  }).then(index => {
    if (index.response == 0) {
      autoUpdater.downloadUpdate();
    }
    else {

    }
  })
})

autoUpdater.on('update-not-available', () => {
  dialog.showMessageBox({
    title: '提示',
    message: '暂无更新'
  })
})

autoUpdater.on('update-downloaded', () => {
  dialog.showMessageBox({
    type: 'info',
    title: '安装更新',
    buttons: ['安装', '稍后安装'],
    message: '安装包已经下载完毕，是否现在安装？',
    cancelId: 1,
  }).then(index => {
    if (index.response == 0) {
      autoUpdater.quitAndInstall()
    }
  })
})

// export this to MenuItem click callback
export default function checkForUpdates(menuItem, focusedWindow, event) {
  autoUpdater.checkForUpdates()
}
