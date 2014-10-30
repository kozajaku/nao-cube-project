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
public class SubpictureBundle {
    
    public int[][] pixels;
    public int topPixelIndex;

    public SubpictureBundle(int[][] pixels, int topPixelIndex) {
        this.pixels = pixels;
        this.topPixelIndex = topPixelIndex;
    }
    
    
}
