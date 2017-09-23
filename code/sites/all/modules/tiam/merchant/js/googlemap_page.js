function initialize()
{
    var myCenter = 0;
    var x = jQuery("#map_position_x").val();
    var y = jQuery("#map_position_y").val();
    if((x == 0 && y == 0) || (x == undefined && y == undefined))
    {
         myCenter = new google.maps.LatLng(32.6295447332617, 51.360626217210665);                  
    }
    else
    {
        myCenter = new google.maps.LatLng(x,y);        
    }
    var mapProp = {
        center: myCenter,
        zoom: 14,
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    var map = new google.maps.Map(document.getElementById("map_location"), mapProp);
    var marker = new google.maps.Marker({
        position: myCenter,
        animation: google.maps.Animation.BOUNCE,
    });
    marker.setMap(map);
}
google.maps.event.addDomListener(window, 'load', initialize);