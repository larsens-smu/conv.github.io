/**
 * The preload script runs before. It has access to web APIs
 * as well as Electron's renderer process modules and some
 * polyfilled Node.js functions.
 * 
 * https://www.electronjs.org/docs/latest/tutorial/sandbox
 */


console.log('in preload log ')
const {ipcRenderer, contextBridge} = require('electron')

contextBridge.exposeInMainWorld('electronAPI', {
    desktop:true,
    saveImage: (filePath, data) => ipcRenderer.send('save-image', filePath, data)

})

window.addEventListener('DOMContentLoaded', () => {
const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
}

for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
    
}
})
 