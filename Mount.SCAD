

translate([60+13,0,37+80])//13
rotate([-90,50,-90])
import("involute_gears.stl");


difference() {


union() {  //base
hull(){
cube([160,60,75]);
translate([62,-29,82])
rotate([00,90,0])
cylinder(68,5,5);
}
translate([35,30,0])
rotate([0,0,45])
cube([30,50,75]);
translate([130,26,70])
cube([30,24,21]);
}



//stuff to remove
translate([-1,-41,-1])
cube([61,80,300]);    //space for register
translate([-1,-1,-1])
cube([24,90,80]);   //baseboard
translate([22,70,15])
screw();
translate([22,70,40])
screw();
translate([22,70,65])
screw();

translate([55,-29,82])
rotate([00,90,0])
cylinder(80,2,2);

translate([0,30,-10])
rotate([45,0,0])
cube([180,30,30]); //Legs

translate([0,30,0])
rotate([45,0,0])
cube([180,30,30]); //legs

translate([0,30,10])
rotate([45,0,0])
cube([180,30,30]); //legs

translate([92,38,75])
rotate([0,90,0])
cylinder(32,20,20); //motor gear

translate([175,38,84])
rotate([0,-90,0])
motor(); //motor hole

//stat gear
translate([72,-30,80])
rotate([0,90,0])
cylinder(47,60,60);

translate([55,2,100])
rotate([0,90,0])
cylinder(40,30,30);
}







module motor() {

translate([0,0,1])
cylinder(45,2,2);
cylinder(43,8.5,8.5);
}

module screw() { //#8 screw

rotate([0,90,0])
{
cylinder(90,2,2);
translate([0,0,7])
cylinder(80,4,4);
}}
