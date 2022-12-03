const { app, BrowserWindow, dialog, Notification, ipcMain, contextBridge} = require('electron')
const fs = require('fs')
var path = require('path');

console.log('in main.js ' + path.join(__dirname, 'preload.js'))

const createWindow = () => {
    const win = new BrowserWindow({
        width: 2400,
        height: 1800,
        webPreferences: {
          preload: path.join(__dirname, 'preload.js')
        }
    })

    win.loadFile('index.html')

    // win.webContents.openDevTools()
}

app.whenReady().then(() => {
    createWindow()
    ipcMain.on('save-image', saveImage)
})

app.commandLine.appendSwitch('enable-features','SharedArrayBuffer')


app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') app.quit()
})


function saveImage(event, data){
    fs.writeFile(path.join(data.filepath, data.filename), data.base64Data, 'base64', function(err) {
        console.log(err);
      });
      
}
