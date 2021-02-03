var fla = "file:///C:/Users/$username$/AppData/Local/EasyStudio/fla.txt"
var targetDir = "file:///D:/fla2csd/";
var doc = fl.getDocumentDOM();

function getDirPath(frameName){
	var pathURI = doc.pathURI
	var pos = pathURI.lastIndexOf('.');//'/所在的最后位置'
	var pos2 = frameName.lastIndexOf('/');
	var dir = pathURI.substring(0, pos) + "_export"+frameName.substring(pos2);
	targetDir = dir
	if(!FLfile.exists(targetDir)){
		FLfile.createFolder(targetDir)
	}
	return dir
}

//@summary get all elements of the current frame
function getMcByFrame(n){
	var elements = new Array();
	var layers = fl.getDocumentDOM().getTimeline().layers;
	each(layers, function (layer, i){
		var frames = layers[i].frames
		var frame = frames[n]
		if (frame){
			each(frame.elements, function(element, index){
				elements.push(element)
			})
		}
	})
	var mcs = filter(elements, function(element){
		return element.elementType == "instance" && element.instanceType == "symbol" && element.symbolType == "movie clip"
	})
	mcs = mcs.sort(function(elem1, elem2){
		return elem1.depth - elem2.depth > 0 ? 1 : -1
	})
	return  mcs
}
//@summary symbol2png
function exportPng2(mc, targetDir, name){
	var bmW = Math.ceil(mc.width);
	var bmH = Math.ceil(mc.height);
	bmW = bmW % 2 == 0 ? bmW : bmW + 1
	bmH = bmH % 2 == 0 ? bmH : bmH + 1
	var document = fl.createDocument();
	document.addItem({x : bmW/2, y : bmH/2}, mc.libraryItem)
	document.width = bmW
	document.height = bmH
	document.exportPNG(targetDir+name+".png",true,true);
	document.close(false);
}

//@summary symbolpng
function exportPng(mc, targetDir, name) {
	var bmW = Math.ceil(mc.width);
	var bmH = Math.ceil(mc.height);
	bmW = bmW % 2 == 0 ? bmW : bmW + 1
	bmH = bmH % 2 == 0 ? bmH : bmH + 1
	var document = fl.createDocument();
	document.addItem({x : bmW/2, y : bmH/2}, mc.libraryItem)
	document.width = bmW
	document.height = bmH
	document.convertToSymbol("graphic", "eClip", "top left")
	var clip = document.selection[0];
	clip.scaleX = 1.0;
	clip.scaleY = 1.0;
	document.convertSelectionToBitmap();
	var bm = document.selection[0];
	bm.x = 0;
	bm.y = 0;
	bmW = Math.ceil(bm.width);
	bmH = Math.ceil(bm.height);
	bmW = bmW % 2 == 0 ? bmW : bmW + 1
	bmH = bmH % 2 == 0 ? bmH : bmH + 1
	document.width = bmW;
	document.height = bmH;
	document.exportPNG(targetDir+"/"+name+".png",true,true);
	document.close(false);
	fl.trace(targetDir+name+".png")
}


function main() {
	if(!FLfile.exists(targetDir)){
		FLfile.createFolder(targetDir)
	}

	var document = fl.getDocumentDOM()
	var timeline = document.getTimeline()
	var curLayerIndex = timeline.currentLayer;
	var layers = timeline.layers
	var currentLayer = layers[curLayerIndex]
	var curFrameIndex = timeline.currentFrame;
	var currentFrame = currentLayer.frames[curFrameIndex]
	var mcs = getMcByFrame(curFrameIndex)
	var frameName = currentFrame.name
	var arr = frameName.split("-")
	var config = {}
	config.symbols = []
	config.path = "res/ui/games/"+frameName
	getDirPath(frameName)
	fl.trace(targetDir)
	var exports = []
	each(mcs, function (mc) {
		if(mc.name == ""){
			// alert("某个元件未命名")
			// return "exit"
			mc.name = mc.libraryItem.name
		}
		var name = mc.name
		var argc = name.split("$")
		var type = "ImageView"
		if (argc[1]) {
			name = argc[0]
			type = argc[1]
		}
		var exportName = name
		var libName = mc.libraryItem.name
		if (libName.indexOf("gg_") < 0) {
			exportName = mc.libraryItem.name
		}
		var position = getPos(mc);
		var dataConfig = {
			name: name,
			image: exportName + ".png",
			type: type || "ImageView",
			x: ""+position.x,
			y: ""+position.y,
			depth: ""+mc.depth,
		}
		config.symbols.push(dataConfig)
		if (!exist(exports, exportName)) {
			exportPng(mc, targetDir, exportName)
			exports.push(exportName)
		}
		mc.visible = false
	})
	document.width = 1440
	document.height = 680
	document.exportPNG(targetDir + "/background.png", true, true);
	config.symbols.unshift({
		name: "background",
		image: "background.png",
		type: "ImageView",
		x: "720",
		y: "340",
	})
	each(mcs, function (mc) { 
		mc.visible = true
	})
	var configPath = targetDir + "/config.json"
	if(FLfile.exists(configPath)){
 		FLfile.remove(configPath)
	}
	if (FLfile.write(configPath, stringify(config))) {  
		var ret = confirm("生成CSD?")
		if (ret) {
			FLfile.write(fla, configPath)
			alert("Success"); 
		}
	}
}

//@summary object2jsonStr
function stringify(arg){
	var c, i, l, s = '', v;
	switch (typeof arg) {
	case 'object':
		if (arg) {
			if (arg instanceof Array) {
				for (i = 0; i < arg.length; ++i) {
					v = stringify(arg[i]);
					if (s) {
						s += ',';
					}
					s += v;
				}
				return '[' + s + ']';
			} else if (typeof arg.toString != 'undefined') {
				for (i in arg) {
					v = arg[i];
					if (typeof v != 'undefined' && typeof v != 'function') {
						v = stringify(v);
						if (s) {
							s += ',';
						}
						s += stringify(i) + ':' + v;
					}
				}
				return '{' + s + '}';
			}
		}
		return 'null';
	case 'number':
		return isFinite(arg) ? String(arg) : 'null';
	case 'string':
		l = arg.length;
		s = '"';
		for (i = 0; i < l; i += 1) {
			c = arg.charAt(i);
			if (c >= ' ') {
				if (c == '\\' || c == '"') {
					s += '\\';
				}
				s += c;
			} else {
				switch (c) {
					case '\b':
						s += '\\b';
						break;
					case '\f':
						s += '\\f';
						break;
					case '\n':
						s += '\\n';
						break;
					case '\r':
						s += '\\r';
						break;
					case '\t':
						s += '\\t';
						break;
					default:
						c = c.charCodeAt();
						s += '\\u00' + Math.floor(c / 16).toString(16) +
							(c % 16).toString(16);
				}
			}
		}
		return s + '"';
	case 'boolean':
		return String(arg);
	default:
		return 'null';
	}
}

function formatPath(path) {
	if (path.indexOf("file:///") < 0) {
		path = "file:///" + path
	}
	if(!FLfile.exists(path)){
		alert("文件或文件夹不存在！！！")
	}
	return path
}

//@summary 获取坐标(此坐标为与中心点的距离)
function getPos(mc){
	return {
		x : Math.round(mc.x * 100 / 100),
		y : Math.round((680 - mc.y) * 100 / 100)
	}
}

function getLen(obj){
	if(obj instanceof Array){
		return obj.length
	}else{
		var count = 0
		for (var i in obj) {
			count = count + 1
		}
		return count
	}
}

//@summary Functional
function each(array, func){
	if(array && array instanceof Array){
		for(var i = 0; i < array.length; i++){
			var ret = func(array[i], i);
			if (ret == "exit") {
				break;
			}
		}
	}
}
function filter(array, func){
	var newArray = new Array();
	if(array && array instanceof Array){
		for(var i = 0; i < array.length; i++){
			var ret = func(array[i], i);
			if(ret){
				newArray.push(array[i]);
			}
		}
	}
	return newArray;
}

function exist(array, data) {
	var ret = false
	each(array, function (item) {
		if (item == data) {
			ret = true
		}
	})
	return ret
}

main();
