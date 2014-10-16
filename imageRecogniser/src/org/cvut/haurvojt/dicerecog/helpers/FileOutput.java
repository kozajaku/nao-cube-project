/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.cvut.haurvojt.dicerecog.helpers;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/**
 *
 * @author haurvojt
 */
public class FileOutput {
    
    public static void writeOutput(int h, int w, int[][] pixels) {
        writeOutput(h, w, pixels, "output.png");
    }

    public static void writeOutput(int h, int w, int[][] pixels, String filename) {
        File output = new File(filename);
        if (output.exists()) {
            output.delete();
        }

        try {
            output.createNewFile();
        } catch (IOException ex) {
            Logger.getLogger(FileOutput.class.getName()).log(Level.SEVERE, null, ex);
            return;
        }

        BufferedImage im = new BufferedImage(w, h, BufferedImage.TYPE_4BYTE_ABGR);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                im.setRGB(j, i, pixels[i][j]);
            }
        }
        try {
            ImageIO.write(im, "PNG", new File(filename));
        } catch (IOException ex) {
            Logger.getLogger(FileOutput.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
