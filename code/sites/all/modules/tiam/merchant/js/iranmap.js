/*
 * Iran Map - SVG and Responsive.
 * Free and open source.
 * Version 1.1.0
 * By: MohammadReza Pourmohammad.
 * Email: mohammadrpm@gmail.com
 * Web: http://mrpm.ir
 */

jQuery(function() {
    jQuery(window).resize(function() {
        resposive();
    });

    function resposive() {
        var height = jQuery('#IranMap').height();
        var width = jQuery('#IranMap').width();
        if (height > width) {
            jQuery('#IranMap svg').height(width).width(width);
        } else {
            jQuery('#IranMap svg').height(height).width(height);
        }
    }
    resposive();

    jQuery('#IranMap .map .province path').click(function() {
        var className = jQuery(this).attr('class');
        var parrentClassName = jQuery(this).parent('g').attr('class');
        var itemID = jQuery('#IranMap .list .' + parrentClassName + ' .' + className + ' a').attr('href');
        window.location.replace(Drupal.settings.basePath+'merchant/list?state='+itemID);
    });

    jQuery('#IranMap').mousemove(function(e) {
        var posx = 0;
        var posy = 0;
        if (!e)
            var e = window.event;
        if (e.pageX || e.pageY) {
            posx = e.pageX;
            posy = e.pageY;
        } else if (e.clientX || e.clientY) {
            posx = e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
            posy = e.clientY + document.body.scrollTop + document.documentElement.scrollTop;
        }
        if (jQuery('#IranMap .show-title').html()) {
            var offset = jQuery(this).offset();
            var x = (posx - offset.left + 25) + 'px';
            var y = (posy - offset.top - 5) + 'px';
            jQuery('#IranMap .show-title').css({'left': x, 'top': y});
        }
    });

    jQuery('#IranMap svg g path').hover(function() {
        var className = jQuery(this).attr('class');
        var parrentClassName = jQuery(this).parent('g').attr('class');
        var itemName = jQuery('#IranMap .list .' + parrentClassName + ' .' + className + ' a').html();
        if (itemName) {
            jQuery('#IranMap .list .' + parrentClassName + ' .' + className + ' a').addClass('hover');
            jQuery('#IranMap .show-title').html(itemName).css({'display': 'block'});
        }
    }, function() {
        jQuery('#IranMap .list a').removeClass('hover');
        jQuery('#IranMap .show-title').html('').css({'display': 'none'});
    });
});