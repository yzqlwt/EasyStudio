function main() {
	var document = fl.getDocumentDOM()
	var timeline = document.getTimeline()
    var layer = timeline.layers[0]
    var frames = layer.frames
    var frame = frames[0]
    frame.actionScript = "stop()\n _root.onLoad = function(){\n _root.gotoAndStop(2)\n }\n"
}
main()