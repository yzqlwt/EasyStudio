
var doc = fl.getDocumentDOM();
var index = 1;

function main(){
	if(!doc ){
		alert("no flash, please open flash.");
		return;
	}
	var selectEles = doc.selection;
	doc.selectNone()
    selectEles = selectEles.sort(function(elem1, elem2){
		return elem1.depth - elem2.depth > 0 ? -1 : 1
	})
	for (var i = 0; i < selectEles.length; i++) {
        var ele = selectEles[i]
		convertToGraphics(ele, "gg_"+i)
	}
}

function convertToGraphics(el, name)
{
    try
    {
        var symbolName = name;

        var cur_lib = fl.getDocumentDOM().library;

        fl.getDocumentDOM().selectNone();
        fl.getDocumentDOM().selection = [el];
        while(cur_lib.itemExists(symbolName))
            symbolName=symbolName+1;
        fl.getDocumentDOM().convertToSymbol("movie clip", symbolName, "center");
    }
    catch (e)
    {
        fl.trace("Exception in : convertToGraphics" + e);
    }
}





main();
