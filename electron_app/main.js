const { app, BrowserWindow, ipcMain } = require("electron")
const path = require("node:path")
const nativeAddon = require("native_addon")

const savedBuffers = [];
const largestBufferSize = 1024*1024*1024;

function allocateMemoryNative() {
    console.log("Allocating memory in native addon")
    return nativeAddon.allocateMemoryBlock()
}

function allocateMemoryMain() {
    console.log("Allocating memory in main process")

    savedBuffers.push(Buffer.alloc(largestBufferSize, 1));
    return {
        "numBlocks": savedBuffers.length,
        "totalNumMB": 0,
        "freeNumMB": 0,
        "usedNumMB": 0
    }
}

const createWindow = () => {
    const win = new BrowserWindow({
      width: 800,
      height: 600,
      webPreferences: {
        preload: path.join(__dirname, "preload.js")
      }
    })

    win.loadFile("index.html")
}

app.whenReady().then(() => {
    ipcMain.handle("allocateMemoryNative", allocateMemoryNative)
    ipcMain.handle("allocateMemoryMain", allocateMemoryMain)
    createWindow()
})