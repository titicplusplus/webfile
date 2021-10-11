img = document.getElementById("image_f");

function size() {
	img.style.maxHeight = window.innerHeight + "px";
	img.style.maxWidth = window.innerWidth + "px";
	img.ondragstart = function () { return false; };

	document.body.style.height = window.innerHeight + "px";

}

function reqListener () {
	img.src = this.responseText;
	name();
}


function next() {
	click_ui();
	var next = arguments[0];
	var oReq = new XMLHttpRequest();

	oReq.onload = reqListener;
	const url = "/next" + img.getAttribute("src") + "?next=" + next;
	oReq.responseType = "text";
	oReq.open("get", url, true);
	oReq.send();

}

function name() {
	img_name = document.getElementById("name_img");	
	img_name.innerText = img.getAttribute("src").substr(10);
}

function click_ui() {
	var arrow = document.getElementById("arrow");
	var result = arrow.style.display;

	if (result == "flex" || result == "")
	{
		arrow.style.display = "none";
	}
	else
	{
		arrow.style.display = "flex";
	}
}

//////////////////////////////

positionX = -1;

function start(event)
{
	if (positionX == -1)
	{
		positionX = event.pageX;
		console.log( positionX );
	}
}



function fin(event)
{
	finch(event.pageX, event.pageY)
}

function startM(event)
{
	if (positionX == -1) 
	{
		positionX = event.touches[0].clientX;
	}
}

function finM(event)
{
	let goX = event.changedTouches[0].clientX;

	finch(goX,goY);
}

function finch()
{
	let x = arguments[0];

	if ( Math.abs(x - positionX) > 100 )
	{
		if (x - positionX > 0)
		{
			click_ui();
			next(-1);
		}
		else
		{
			click_ui();
			next(1);
		}
		
	}
	else
	{
		click_ui();
	}
	
	positionX = -1;
}



//////////////////////////////

size();
name();

window.onresize = size;
