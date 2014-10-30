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
public class DarkDiceDotRecognizer implements DiceDotRecognizer {

    private static final double KOEF = 1.4; 
    @Override
    public boolean isDotColor(int pixel) {
        int red = (pixel >> 16) & 0xff;
        int green = (pixel >> 8) & 0xff;
        int blue = (pixel) & 0xff;

        boolean sameColor = red < green * KOEF && KOEF * red > green &&
                            red < blue * KOEF && KOEF * red > blue &&
                            blue < green * KOEF && KOEF * blue > green;
        
        return red + blue + green < 180 || (red < 120 && sameColor);
                
    }
    
}
