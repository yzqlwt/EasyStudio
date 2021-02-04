
var doc = fl.getDocumentDOM();

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


function main(){
	if(!doc ){
		alert("no flash, please open flash.");
		return;
	}
	var targetDir = getDirPath()
	if(!FLfile.exists(targetDir)){
		FLfile.createFolder(targetDir)
	}
	var selectEles = doc.selection;
	doc.selectNone()
	var mcs = []
	if (selectEles.length > 0) {
		mcs = filter(selectEles, function(element){
			return element.elementType == "instance" && element.instanceType == "symbol" && element.symbolType == "movie clip"
		})
	} else {
		var timeline = doc.getTimeline()
		var curLayerIndex = timeline.currentLayer;
		var layers = timeline.layers
		var currentLayer = layers[curLayerIndex]
		var curFrameIndex = timeline.currentFrame;
		var currentFrame = currentLayer.frames[curFrameIndex]
		mcs = getMcByFrame(curFrameIndex)
	}
	each(mcs, function (mc) {
		exportPng(mc, targetDir)
		mc.visible = true
	})
}

//@summary symbol2png
function exportPng(mc, targetDir) {
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
	var name = mc.name
	if (name == "") {
		name = mc.libraryItem.name
	}
	document.exportPNG(targetDir+name+".png",true,true);
	document.close(false);
	fl.trace(mc.libraryItem.name)
}

function exportPng2(ele, targetDir){
	if (ele.width < 1500) {
		if (ele.height < 800) {
			ele.selected = true
			doc.clipCopy();
			var exportDoc = fl.createDocument();
			exportDoc.clipPaste();
			exportDoc.selectAll();
			exportDoc.convertToSymbol("graphic", "eClip", "top left")
			var clip = exportDoc.selection[0];
			clip.scaleX = 1;
			clip.scaleY = 1;
			exportDoc.convertSelectionToBitmap();
			var bm = exportDoc.selection[0];
			bm.x = 0;
			bm.y = 0;
			bmW = Math.ceil(bm.width);
			bmH = Math.ceil(bm.height);
			bmW = bmW % 2 == 0 ? bmW : bmW + 1
			bmH = bmH % 2 == 0 ? bmH : bmH + 1
			exportDoc.width = bmW;
			exportDoc.height = bmH;
			var timeStr = getTimeStr();
			var pngName = targetDir+mc.name+".png"
		 	index = index + 1;
			exportDoc.selectNone();
			exportDoc.exportPNG(pngName,true,true);
			exportDoc.close(false);
			ele.selected = false
		}
	}
}

function getTimeStr(){
	var today = new Date();
	var y = checkTime(today.getFullYear());
	var m = checkTime(today.getMonth() + 1);
	var d = checkTime(today.getDate())
	var h2 = checkTime(today.getHours())
	var m2 = checkTime(today.getMinutes())
	var s2 = checkTime(today.getSeconds())
	return y + m + d + h2 + m2 + s2;
}

function checkTime(i) {
	if (i < 10) i = "0" + i;
	return i.toString()
}

function getDirPath(){
	var pathURI = doc.pathURI
	var pos = pathURI.lastIndexOf('.');//'/所在的最后位置'
	var targetDir = pathURI.substring(0, pos) + "_images/";
	return targetDir
}

//@summary Functional
function each(array, func){
	if(array && array instanceof Array){
		for(var i = 0; i < array.length; i++){
			func(array[i], i);
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

main();
