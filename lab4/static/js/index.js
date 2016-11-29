/*
 * Ok, i lied. There are like three lines! And I could
 * probably get rid with them if i converted
 * the transitions to animations.
*/
$(document).ready(function() {
    $('html').addClass('load-complete');
});

$('#play').click(function(){
	$.ajax({
		url: 'http://localhost:5000/toggle',
		type: 'GET',
		data: {'submit':true},
		success: function(result){
			alert(result);
		}
	});
});

$("#r5432").click(function(){
	$.ajax({
		url: "http://localhost:5000/change?port=5432",
		type: "GET",
		success: function(result){
			alert(result);
		}
	})
})

$("#r5433").click(function(){
	$.ajax({
		url: "http://localhost:5000/change?port=5433",
		type: "GET",
		success: function(result){
			alert(result);
		}
	})
})

$("#r5434").click(function(){
	$.ajax({
		url: "http://localhost:5000/change?port=5434",
		type: "GET",
		success: function(result){
			alert(result);
		}
	})
})

$("#r5435").click(function(){
	$.ajax({
		url: "http://localhost:5000/change?port=5435",
		type: "GET",
		success: function(result){
			alert(result);
		}
	})
})

$("#r5436").click(function(){
	$.ajax({
		url: "http://localhost:5000/change?port=5436",
		type: "GET",
		success: function(result){
			alert(result);
		}
	})
})