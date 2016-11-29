/*
 * Ok, i lied. There are like three lines! And I could
 * probably get rid with them if i converted
 * the transitions to animations.
*/
$(document).ready(function() {
    $('html').addClass('load-complete');
});

$('.tool-buttons').click(function(){
	$.ajax({
		url: 'localhost:5000/toggle',
		type: 'POST',
		data: {'submit':true},
		success: function(result){
			alert("DONE!");
		}
	});
});