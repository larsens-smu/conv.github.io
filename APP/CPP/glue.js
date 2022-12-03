
// Bindings utilities

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare: function() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc: function(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = (len + 7) & -8; // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy: function(array, view, offset) {
    offset >>>= 0;
    var bytes = view.BYTES_PER_ELEMENT;
    switch (bytes) {
      case 2: offset >>>= 1; break;
      case 4: offset >>>= 2; break;
      case 8: offset >>>= 3; break;
    }
    for (var i = 0; i < array.length; i++) {
      view[offset + i] = array[i];
    }
  },
};

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}
/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}


// VoidPtr
/** @suppress {undefinedVars, duplicate} @this{Object} */function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

  VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};
// Conv
/** @suppress {undefinedVars, duplicate} @this{Object} */function Conv() {
  this.ptr = _emscripten_bind_Conv_Conv_0();
  getCache(Conv)[this.ptr] = this;
};;
Conv.prototype = Object.create(WrapperObject.prototype);
Conv.prototype.constructor = Conv;
Conv.prototype.__class__ = Conv;
Conv.__cache__ = {};
Module['Conv'] = Conv;

Conv.prototype['setKernal'] = Conv.prototype.setKernal = /** @suppress {undefinedVars, duplicate} @this{Object} */function(size, name) {
  var self = this.ptr;
  ensureCache.prepare();
  if (size && typeof size === 'object') size = size.ptr;
  if (name && typeof name === 'object') name = name.ptr;
  else name = ensureString(name);
  _emscripten_bind_Conv_setKernal_2(self, size, name);
};;

Conv.prototype['applyFilter'] = Conv.prototype.applyFilter = /** @suppress {undefinedVars, duplicate} @this{Object} */function(rows, cols, imgVoid, channels, padding, new_imgVoid, num_threads) {
  var self = this.ptr;
  if (rows && typeof rows === 'object') rows = rows.ptr;
  if (cols && typeof cols === 'object') cols = cols.ptr;
  if (imgVoid && typeof imgVoid === 'object') imgVoid = imgVoid.ptr;
  if (channels && typeof channels === 'object') channels = channels.ptr;
  if (padding && typeof padding === 'object') padding = padding.ptr;
  if (new_imgVoid && typeof new_imgVoid === 'object') new_imgVoid = new_imgVoid.ptr;
  if (num_threads && typeof num_threads === 'object') num_threads = num_threads.ptr;
  return _emscripten_bind_Conv_applyFilter_7(self, rows, cols, imgVoid, channels, padding, new_imgVoid, num_threads);
};;

  Conv.prototype['__destroy__'] = Conv.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_Conv___destroy___0(self);
};
// ConvHelper
/** @suppress {undefinedVars, duplicate} @this{Object} */function ConvHelper() {
  this.ptr = _emscripten_bind_ConvHelper_ConvHelper_0();
  getCache(ConvHelper)[this.ptr] = this;
};;
ConvHelper.prototype = Object.create(WrapperObject.prototype);
ConvHelper.prototype.constructor = ConvHelper;
ConvHelper.prototype.__class__ = ConvHelper;
ConvHelper.__cache__ = {};
Module['ConvHelper'] = ConvHelper;

ConvHelper.prototype['toFloats'] = ConvHelper.prototype.toFloats = /** @suppress {undefinedVars, duplicate} @this{Object} */function(img, length, answer) {
  var self = this.ptr;
  if (img && typeof img === 'object') img = img.ptr;
  if (length && typeof length === 'object') length = length.ptr;
  if (answer && typeof answer === 'object') answer = answer.ptr;
  return _emscripten_bind_ConvHelper_toFloats_3(self, img, length, answer);
};;

ConvHelper.prototype['fromFloats'] = ConvHelper.prototype.fromFloats = /** @suppress {undefinedVars, duplicate} @this{Object} */function(img, length, answer) {
  var self = this.ptr;
  if (img && typeof img === 'object') img = img.ptr;
  if (length && typeof length === 'object') length = length.ptr;
  if (answer && typeof answer === 'object') answer = answer.ptr;
  return _emscripten_bind_ConvHelper_fromFloats_3(self, img, length, answer);
};;

  ConvHelper.prototype['__destroy__'] = ConvHelper.prototype.__destroy__ = /** @suppress {undefinedVars, duplicate} @this{Object} */function() {
  var self = this.ptr;
  _emscripten_bind_ConvHelper___destroy___0(self);
};