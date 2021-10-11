function reqListener () {
	result = this.responseText;
	
	path = document.getElementById("path");
	path.innerHTML = result;

}

function change()
{
	str = s.value;

	if (str.length == 0)
		return false;

	var oReq = new XMLHttpRequest();
	oReq.onload = reqListener;
	oReq.responseType = 'text';
	oReq.open("get", "/sdir/" + str, true);
	oReq.send();	

	return false;
}

s = document.getElementById("s");
b = document.getElementById("b");
b.addEventListener('click', change);
