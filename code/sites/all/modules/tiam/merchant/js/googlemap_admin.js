function initialize()
{
    var myCenter = 0;
    var x = jQuery("#map_position_x").val();
    var y = jQuery("#map_position_y").val();
    if(x == 0 && y == 0)
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
    });
    marker.setMap(map);
    google.maps.event.addListener(map,'click',function(event) {
        marker.setMap(map);
        marker.setPosition(event.latLng);
        document.getElementById('map_position_x').value = event.latLng.lat();
        document.getElementById('map_position_y').value = event.latLng.lng();
    });
}
google.maps.event.addDomListener(window, 'load', initialize);