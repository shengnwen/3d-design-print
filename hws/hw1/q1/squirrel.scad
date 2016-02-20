module squirrel(res)
{
    rotate([0, 10, 0])
    translate([6, 0, 14.5])
    head(res*1.5);
    hull(){
    translate([8, 0, 10])
    rotate([0, 25, 0])
    cylinder(h=0.7, r1 = 1.5, r2 = 1.2, $fa=10, $fn=20);
    translate([2, 0, 0])
    rotate([0, 25, 0])
    body(res);
    }
    translate([3, 4, -3])
    legs(res*0.8);
    translate([3, -4, -3])
    legs(res*0.8);
    translate([10, 0, 5])
    cone(res);
    translate([10, -1, 5])
    scale([0.5, 0.5, 0.5])
    rotate([80,-20,66])
    hand(res);
    
    translate([10, 0, 5])
    scale([0.5, 0.5, 0.5])
    rotate([80,-20,0])
    hand(res);
    translate([-2, 0, 6])
    scale([0.5, 0.3, 0.5])
    rotate([0, -90, 0])
    tail(res);
}

module body(res){
        minkowski(){
            polyhedron(
  points=[ [res,res,0],[res,-res,0],[-res,-res,0],[-res,res,0], // the four points at base
           [0,0,res*4]  ],                                 // the apex point 
  faces=[ [0,1,4],[1,2,4],[2,3,4],[3,0,4],              // each triangle side
              [1,0,3],[2,1,3] ]                         // two triangles for square base
 );
            sphere(r = res*1.5, $fa = 6, $fn = 50);
        }
}
module oval(w,h, height, center = false) {
 scale([1, h/w, 1]) cylinder(h=height, r=w, center=center);
}

module legs(res){
    scale([1, 0.8, 1])
    hull(){
    translate([0, 0, res*2])
    sphere(r=res*1.2);
    oval(res, res*1.5, res*2.4);
    translate([0, 0, res*0.1])
    sphere(r=res*1);
    };
    translate([res*0.8, 0, -res*0.4])
    finger(res*0.28);
    rotate([0, 0, 15])
    translate([res*0.7, res*0.1, -res*0.4])
    finger(res*0.25);
    rotate([0, 0, -15])
    translate([res*0.7, -res*0.1, -res*0.4])
    finger(res*0.25);
 }
 module finger(res){
     res = res;
     rotate([0, 90, 0]){
         hull(){
    polyhedron(
  points=[ [res,res,0],[res,-res,0],[-res,-res,0],[-res,res,0], // the four points at base
           [0,0,res*5]  ],                                 // the apex point 
  faces=[ [0,1,4],[1,2,4],[2,3,4],[3,0,4],              // each triangle side
              [1,0,3],[2,1,3] ]                         // two triangles for square base
 );
    sphere(r=res*1.3);
    };
};
}
module head(res){

    translate([-0.5,1.,3])
    rotate([20, 35, 90])
    polyhedron(
  points=[ [2,0,0], [0, 1, 0], [0, -1, 0], [0, 0, 2] ],                                 // the apex point 
  faces=[[0, 1, 2], [0, 1, 3], [0, 2, 3], [1, 2, 3] ]                         // two triangles for square base
 );
    translate([-0.5,-1,3])
    rotate([-20, 35, -90])
    polyhedron(
  points=[ [2,0,0], [0, 1, 0], [0, -1, 0], [0, 0, 2] ],                                 // the apex point 
  faces=[[0, 1, 2], [0, 1, 3], [0, 2, 3], [1, 2, 3] ]                         // two triangles for square base
 );
    color("black")
    translate([res * 1, res *0.95, res * 0.1]) {
        rotate([20,0,0])
        rotate([0,0,140])
        cube(size = [res * 2.2, res*0.02, res*0.02], center = true);
    }
    color("black")
    translate([res * 1, -res * 0.95, res * 0.1]) {
        rotate([-20,0,0])
        rotate([10,0,-140])
        cube(size = [res * 2.2, res*0.02, res*0.02], center = true);
    }
    color("black")
    translate([res * 1, res *0.95, 0]) {
        rotate([-10,0,0])
        rotate([0,0,140])
        cube(size = [res * 1.8, res*0.02, res*0.02], center = true);
    }
    color("black")
    translate([res * 1, -res * 0.95, 0]) {
        rotate([-10,0,0])
        rotate([10,0,-140])
        cube(size = [res * 1.8, res*0.02, res*0.02], center = true);
    }
    hull(){
    scale([1.4, 1.2, 1.2])
    sphere(r = res, $fn = 40);
    translate([res * 1.2, 0, -res * 0.1])
    sphere(r = res * 0.7, $fn = 40);
    translate([res * 1.2, 0, -res * 0.2])
    sphere(r = res * 0.7, $fn = 40);
    }
    color("black")
    translate([res * 0.1, res * 1.1, res * 0.32])
    sphere(r = res * 0.3);
    color("black")
    translate([res * 0.1, -res * 1.1, res * 0.32])
    sphere(r = res * 0.3);
}
module hand(res) {
    translate([-res*2.8, res*1.2, -res*2.2])
    scale([1, 2, 1])
    sphere(r=res * 1.2, $fn = 50);
    rotate([0, -130, 0]){
    hull(){
    cylinder(h = 7, r1 = 1.8, r2= 2, $fn = 40);
    sphere(r=res);
    }
    rotate([0, 100, 0])
    translate([1.5, 0, 0])
    finger(res*0.3);
    rotate([-20, 90, 0])
    translate([1.2, +1, 0])
    finger(res*0.3);
    rotate([20, 90, 0])
    translate([1.2, -1, 0])
    finger(res*0.3);
    }
}
module cone(res) {
//    translate([0, 0, res * 0.3])
//    cube(size=[res*0.1, res * 0.1, res * 0.2], center = true)
    translate([0, 0, res * 0.5]){
    cylinder(h = res * 0.2, r1 = res * 0.1, r2 = res*0.1, center=true, $fa = 10, $fs = 10);
    }
    difference(){
        translate([0, 0, -res * 0.2])
        sphere(r = res * 0.6, $fn = 10);
        translate([-res * 0.6, -res * 0.6, -res * 1.2])
        cube(size=res * 1.2);
    };
    translate([0, 0, res * 0.1])
    rotate([180, 0, 0]){
    cylinder(h = res, r1 = res * 0.51, r2=0, center = false);
    }
    
    
}
module tail(res){
    hull(){
    cylinder(1, 4, 3, $fn = 100);
    translate([3, 0, 18])
    rotate([0, 45, 0])
    scale([1, 1, 2])
    sphere(r=4, $fn = 50);
    }
    translate([-2.4, 0, -0.4])
    rotate([0, 90, 0])
    scale([1, 1, 1.6])
    sphere(r = 4, $fn = 100);
}

squirrel(2);
