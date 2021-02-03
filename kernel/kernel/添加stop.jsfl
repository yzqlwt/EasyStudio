function main() {
	var document = fl.getDocumentDOM()
	var timeline = document.getTimeline()
    var layer = timeline.layers[0]
    var frames = layer.frames
    each(frames, function (frame, index) { 
        frame.actionScript = "stop()"
    })
}

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
main()