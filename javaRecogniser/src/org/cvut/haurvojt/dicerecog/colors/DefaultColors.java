/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.cvut.haurvojt.dicerecog.colors;

/**
 *
 * @author haurvojt
 */
public class DefaultColors {
    
    /**
     * This color is used in preprocessing to fill the areas around the body of found DiceCluster.
     */
    public static final int OUTSIDE_FILLING_COLOR = 0xffffffff;
    
    /**
     * This color is used in clustering to fill the parts that did not pass for the color of the cube 
     * and neither the color for the dots. <b>Must</b> be 
     */
    public static final int INSIDE_FILLING_COLOR = 0xffff0000;
}
