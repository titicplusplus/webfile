stylev = 1;

function gs()
{
	style = document.getElementById("style");
	style.href = "/file/style" + arguments[0] + ".css";
	
	if (arguments[0] == '')
	{
		stylev = 1;
	}
	else
	{
		stylev = 2;

	}
}

function go_dir()
{
	console.log( arguments[0] );
	document.location.href= "/dir/" + arguments[0] + "?a=" + stylev;

}

function back()
{
	s = window.location.href;

	if (s == window.location.origin + "/")
		return;

	var num  = s.lastIndexOf("/");
	s = s.substr(0,  num );

	var num  = s.lastIndexOf("/");
	s = s.substr(0, num);
	
	if (s.substr( s.lastIndexOf("/") ) == "/dir")
	{
		s = "";
	}

	document.location.href = s + "/?a=" + stylev;
}

function search()
{
	document.location.href = "/search";
}

