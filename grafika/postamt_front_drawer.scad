include <bosl2/std.scad>
$fn = 80;

eps = 0.01;

total_z = 7.5;
total_y = 60;
total_x = 187;
roffset_x1 = 36;
roffset_x2 = 172;

printer_x = 103.5;
printer_y = 57;

rfid_up = 14;

difference() {
    cuboid([total_x,total_z,total_y], anchor = BOTTOM+RIGHT+FWD);

    fwd(eps)
    up(rfid_up+1) {

    left(roffset_x1)
    union() {
        up(7.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
        up(32.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);    
    }

    left(roffset_x2)
    union() {
        up(7.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
        up(32.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
    }
    }

    fwd(eps)
    up(10)
    left(roffset_x1+10)
    cuboid([roffset_x2-roffset_x1-20,total_z-2+eps,total_y-20], anchor = BOTTOM+RIGHT+FWD);

}

back(eps+total_z-2)
left(roffset_x1 + (roffset_x2-roffset_x1)/2)
up(total_y/2)
cuboid([60,10+eps,10], anchor = BACK);
