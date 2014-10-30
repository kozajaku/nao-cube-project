/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.cvut.haurvojt.dicerecog.core;

import java.awt.Point;

/**
 *
 * @author haurvojt
 */
public class Ellipse {

    public final double angle, a, b, x0, y0;

    public Ellipse(double cubeHeight, double cubeWidth, int leftmostRed, int topRed) {

        angle = Math.tanh(Distance.CAMERA_HEIGHT / Distance.DICE_DISTANCE);
        double bT = 0.5 * (cubeHeight * Math.sin(angle) / (Math.sin(angle) + Math.cos(angle)));
        double aT = cubeWidth / 2.0;

        a = aT * 0.9;
        b = bT * 0.95;

        x0 = leftmostRed + cubeWidth / 2.0;
        y0 = topRed + b;
    }
    
    
    public boolean isPointInside(Point point){
        double value = (point.y - x0) * (point.y - x0) / (a * a) + (point.x - y0) * (point.x - y0) / (b * b);
        return value < 1.05;
    }
    

}
