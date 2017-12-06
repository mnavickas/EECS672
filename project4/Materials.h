#ifndef MATERIALS_H
#define MATERIALS_H

#include "PhongMaterial.h"


#define PHONG_NONE          PhongMaterial( 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 )
#define PHONG_BRICK         PhongMaterial( 0.05,0.05,0.05,              0.3,0.15,0.2,                   0.3/10,0.15/10,0.2/10,          2,1 )
#define PHONG_LOG1          PhongMaterial( 184.0/255,131.0/255,9.0/255, 184.0/255,131.0/255,11.0/255,   0.0,0.0,0.0,                    2,1 )
#define PHONG_LOG2          PhongMaterial( 130.0/255,81.0/255,1.0/255,  130.0/255,81.0/255,1.0/255,     0.01,0.0,0.0,                   2,1 )
#define PHONG_LOG3          PhongMaterial( 130.0/255,81.0/255,1.0/255,  90.0/255,81.0/255,1.0/255,      0.01,0.0,0.0,                   2,1 )
#define PHONG_TABLETOP      PhongMaterial( 0.2125,0.1275,0.054,         0.714,0.4284,0.18144,           0.2,0.1,0.05,                   30,1 )
#define PHONG_TABLETOP_OLD  PhongMaterial( 0.2125,0.1275,0.054,         0.714,0.4284,0.18144,           0.2,0.1,0.05,                   30,1 )
#define PHONG_TABLELEG      PhongMaterial( 0.329412,0.223529,0.027451,  0.780392,0.568627,0.113725,     0.992157,0.941176,0.807843,     27.8974, 1 )
#define PHONG_BED           PhongMaterial( 0.3,0.0,0.0,                 0.0,0.0,0.7,                    0.0,0.0,0.3,                    10, 1 )
#define PHONG_PILLOW        PhongMaterial( 0.9,0.9,0.9,                 0.9,0.9,0.9,                    0.01,0.01,0.01,                 1, 1 )
#define PHONG_CYL           PhongMaterial( 0.3,0.3,0.3,                 0.5,0.5,0.5,                    0.3,0.2,0.01,                   10, 1 )
#define PHONG_WALL          PhongMaterial( 0.3,0.3,0.3,                 0.5,0.5,0.5,                    0.3,0.2,0.01,                   15, 1 )
#define PHONG_FLOOR         PhongMaterial( 0.4,0.4,0.4,                 0.4,0.4,0.4,                    0.6,0.6,0.6,                    20, 1 )
#define PHONG_GRASS         PhongMaterial( 0.0,0.4,0.0,                 0.0,0.4,0.0,                    0.05,0.05,0.05,                 1, 1 )
#define PHONG_WATER         PhongMaterial( 0.0,0.1,0.4,                 0.1,0.4,0.7,                    0.4,0.85,0.6,                   15, 1 )
#define PHONG_FIRE          PhongMaterial( 0.9,0.4,0.0,                 0.9,0.4,0.0,                    0.0,0.0,0.0,                    60, 1 )
#define PHONG_GLASS         PhongMaterial( 0.9,0.9,0.9,                 0.78,0.78,0.8,                  0.9,0.9,0.9,                    15, 0.3 )

#endif
