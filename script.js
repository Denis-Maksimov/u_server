// // $=jQuery

// $(
//     function(){
//         $("h1").html("Text");
//         $("h1").css("color","green");
//     }
// );

// $("#start").click(
//     function(){
//         $("#msg")
//             .html("jQuerry is working!")
//             .css("background-collor","green")
//             .parent() // родитель
//                 .css("background-color","fff4dd")
//                 .width(150)
//                 .height(80)


//     }
      
// );


// $("#stop").click(
//     function(){
//         location.reload();
//     }
// );


$(document).ready(function(){
    $(".btn-slide").click(function(){
        $("#panel").slideToggle('slow');
        $(this).toggleClass("active");
    });
});
