function initialize()
{
    var myCenter = myCenter = new google.maps.LatLng(32.623732877538146, 51.63187801816093);
    var mapProp = {
        center: myCenter,
        zoom: 16,
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    var map = new google.maps.Map(document.getElementById("googleMap"), mapProp);
    var marker = new google.maps.Marker({
        position: myCenter,
        animation: google.maps.Animation.BOUNCE,
    });
    marker.setMap(map);
}
google.maps.event.addDomListener(window, 'load', initialize);