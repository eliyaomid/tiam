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
        var province = jQuery(this).attr('class');
        window.location.replace(Drupal.settings.basePath+'merchant?state='+province);
    });
});