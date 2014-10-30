/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.cvut.haurvojt.dicerecog.core;

/**
 *
 * @author haurvojt
 */
public class Distance {
    /**
     * Vertical distance from the camera to the floor
     */
    public static double CAMERA_HEIGHT = 28;
    /**
     * Horizontal distance of the center of the base to the robots eyes [cm]
     */
    public static double DICE_DISTANCE = 50;
    
    private static final int DISTANCE_FOR_30 = 530;
//    private static final int DISTANCE_FOR_50 = 270;
    
    public static void setDiceDistance(int topPoint){

        
        DICE_DISTANCE = 30 - (topPoint - DISTANCE_FOR_30) / 13;
        
//        System.out.println("DICE_DISTANCE: " + DICE_DISTANCE);
        
    }
}
