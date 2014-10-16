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
public class RedDiceColorRecognizer implements DiceColorRecognizer {

    @Override
    public boolean isDiceColor(int pixel) {
        int red = (pixel >> 16) & 0xff;
        int green = (pixel >> 8) & 0xff;
        int blue = (pixel) & 0xff;

        return red + green + blue > 100 && red > 0.8 * (green + blue);
    }
    
}
