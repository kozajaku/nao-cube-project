/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.cvut.haurvojt.dicerecog.colors;

/**
 * Interface that must be implemented to help the algorithm recognize the color of the dots on the dice. Please note, 
 * that the implemented class must be used in the class {@link Common} where it is used from all over the algorithm.
 * @author haurvojt
 */
public interface DiceDotRecognizer {
    public boolean isDotColor(int color);
}
