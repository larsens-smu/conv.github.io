

var rows = 1080
var cols = 720
var num_threads = 8
var options = {}
var conv = {}
var convHelper = {}
var filepath = '/Users/stevenlarsen/images'
var filename = 'image1'
var constraints = {}
var firstTime = false;

var encodeArray = function(arr){
    var b64encoded = btoa(String.fromCharCode.apply(null, arr));
    return b64encoded
}
var decodeArray = function(b64encoded){
    var u8_2 = new Uint8Array(atob(b64encoded).split("").map(function(c) {
    return c.charCodeAt(0); }));
    return u8_2
}

//once the document is loaded
document.addEventListener("DOMContentLoaded", () => {
    if(!window.electronAPI){
        const input1 = document.getElementById('filename');
        const input2 = document.getElementById('filepath');
        input1.remove();
        input2.remove();
    }
    

    var kernalSelect = document.getElementById("kernal-select")
    kernalSelect.addEventListener('change',
    function (e) {
        options.kernal = kernalSelect.value
        conv.setKernal(options.kernal, options.kernalSize)
    }, false
    )
    var kernalSizeSelect = document.getElementById("kernal-size-select")
    kernalSizeSelect.addEventListener('change',
    function (e) {
        options.kernalSize = kernalSizeSelect.value
        conv.setKernal(options.kernalSize, options.kernalSize)
    }, false
    )
    var thread_select = document.getElementById("thread-select")
    thread_select.addEventListener('change',
    function (e) {
        options.num_threads = thread_select.value
        num_threads = thread_select.value
    }, false
    )
    var save_save_button = document.getElementById("save-image")
    save_save_button.addEventListener("click", function(){
        const url = canvas.toDataURL('image/jpg', 0.8);

        // remove Base64 stuff from the Image
        const base64Data = url.replace(/^data:image\/png;base64,/, "");

        const NOTIFICATION_TITLE = 'Convolution Finished'
        const NOTIFICATION_BODY = "This is a notification"

        console.log(window.electronAPI)

        if(window.electronAPI){

            window.electronAPI.saveImage({
                filename: document.getElementById("filename").value + ".png", 
                filepath: document.getElementById("filepath").value,
                base64Data: base64Data
            })
            let notif = new Notification("FileSaved", { body: filepath})
        }
        //in the web.
        else{
            var blob = new Blob([decodeArray(base64Data)], {type: "image/png"});
            const url = URL.createObjectURL(blob);
            const link = document.createElement('a');
            link.setAttribute('href', url);
            link.setAttribute('download', "image");
            link.style.visibility = 'hidden';
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);
        }
    })
});
//Once the library is loaded.
Module['onRuntimeInitialized'] = async function () {
    console.log('in module inited')

    conv = new Module.Conv();
    convHelper = new Module.ConvHelper();

    constraints = {
        audio: true,
        video: {
            width: { ideal: 1080 },
            height: { ideal: 720 }
        }
    }
    navigator.mediaDevices.getUserMedia(constraints).then((mediaStream) => {
        const video = document.querySelector('video');
        video.srcObject = mediaStream;
        video.onloadedmetadata = () => {
            video.play();
            options = processor.setUp();
            processor.doLoad();
        };
    })
    .catch((err) => {
        // always check for errors at the end.
        console.error(`error getting media devices ${err.name}: ${err.message}`);
    });
}

//Functions to help process images 
const processor = {
    timerCallback() {

        if (this.video.paused || this.video.ended) {
            return;
        }
        this.computeFrame();
        setTimeout(() => {
            this.timerCallback();
        }, 15); 
    },

    doLoad() {

        this.video = document.getElementById("video");
        this.c1 = document.getElementById("canvas");
        this.ctx1 = this.c1.getContext("2d");

        this.video.addEventListener("play", () => {
            this.width = this.video.width;
            this.height = this.video.height;
            this.timerCallback();
        }, false);
    },

    computeFrame() {
        this.ctx1.drawImage(this.video, 0, 0, this.width, this.height);
        const frame = this.ctx1.getImageData(0, 0, this.width, this.height);
        const l = frame.data.length;

        conv.setKernal(this.kernalSize, this.kernal);
        var image = removeAlpha(frame.data)
        Module.HEAP8.set(image, options.imageCharHeap);

        var timeFirstConverion = convHelper.toFloats(
            options.imageCharHeap,
            (options.rows * options.cols * options.channels),
            options.imageFloatHeap);

        var newImage = applyFilter(conv,
            convHelper,
            image,
            options.rows,
            options.channels,
            options.cols,
            options.kernal,
            options.kernalSize,
            options.imageFloatHeap,
            options.appliedImageFloat,
            options.newImageHeap);


        var localCopyFinal = new Uint8Array(Module.HEAP8.subarray(options.newImageHeap, options.newImageHeap + (options.rows * options.cols * options.channels)));
        ///apply the filter
        var imageIndex = 0;
        for (let i = 0; i < l; i++) {
            if (i % 4 != 3) {
                frame.data[i] = localCopyFinal[imageIndex];
                imageIndex++;
            }
        }

        this.ctx1.putImageData(frame, 0, 0);

        firstTime = false;
        return;
    },
    setUp() {
        return {
            kernalSize: 3,
            cols: 1080,
            rows: 720,
            channels: 3,
            kernal: "sharpen",
            imageCharHeap: Module._malloc((rows * cols * 3) * Uint8Array.BYTES_PER_ELEMENT),
            imageFloatHeap: Module._malloc((rows * cols * 3) * Float32Array.BYTES_PER_ELEMENT),
            appliedImageFloat: Module._malloc((rows * cols * 3) * Float32Array.BYTES_PER_ELEMENT),
            newImageHeap: Module._malloc((rows * cols * 3) * Uint8Array.BYTES_PER_ELEMENT),
        }
    }
};
var applyFilter = function (conv, convHelper, image, rows, channels, cols, filter, size, imageFloatHeap, newImageFlaotHeap, newImageCharHeap) {

    var time = 0.0;
    //imageFloatHeap is the image in one array of floats.
    conv.setKernal(size, filter);
    time += conv.applyFilter(rows, cols, imageFloatHeap, channels, 1.0, newImageFlaotHeap, num_threads)

    //floats to ints
    convHelper.fromFloats(newImageFlaotHeap, (rows * cols * channels), newImageCharHeap);

    var localCopyFinal = new Uint8Array(Module.HEAP8.subarray(newImageCharHeap, newImageCharHeap + image.length));

    return localCopyFinal;
}
var removeAlpha = function (img) {
    let length = img.length;
    let new_length = (img.length / 4) * 3;
    let new_img = new Uint8Array(new_length)
    let j = 0;
    for (let i = 0; i < length; i++) {
        if (i % 4 != 3) {
            new_img[j] = img[i];
            j++;
        }
    }
    return new_img;
}