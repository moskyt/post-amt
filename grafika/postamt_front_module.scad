include <bosl2/std.scad>
$fn = 80;

eps = 0.01;

total_z = 7.5;
total_y = 60;

printer_lip_x = 5;
board_left = 16;

printer_x = 103.5;
printer_y = 57;

rfid_up = 14;

difference() {
    union() {
        difference() {
            cuboid([218,total_z,total_y], anchor = BOTTOM+LEFT+FWD);

            right(board_left+printer_lip_x)
            fwd(eps)
            down(eps)
            cuboid([printer_x,total_z+eps*2,printer_y+eps], anchor = BOTTOM+LEFT+FWD);
            
            right(133)
            up(rfid_up)
            back(1)
            cuboid([65,total_z,42], anchor = BOTTOM+LEFT+FWD);
        }    
        right(133+1+6)
        union() {
            up(rfid_up+1+7.5)
            ycyl(d = 7, l = 4, anchor = FWD);
            up(rfid_up+1+32.5)
            ycyl(d = 7, l = 4, anchor = FWD);
        }
    }

    fwd(eps-1)
    right(170)
    up(7)
    ycyl(d = 12.2, l = total_z+2*eps, anchor = FWD);

    fwd(eps)
    right(170)
    up(7)
    intersection() {
       ycyl(d = 12.2, l = total_z+2*eps, anchor = FWD);
       fwd(eps)
        zcopies(2,5)
       cuboid([16,16,1+2*eps], anchor = FWD);

    }

    fwd(eps)
    up(rfid_up+1) {
    right(board_left/2)
    union() {
        up(7.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
        up(32.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
    }

    right(201)
    union() {
        up(7.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
        up(32.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
    }

    right(133+1+6)
    union() {
        up(7.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
        up(32.5)
        ycyl(d = 4, l = total_z+2*eps, anchor = FWD);
    }
}
}

