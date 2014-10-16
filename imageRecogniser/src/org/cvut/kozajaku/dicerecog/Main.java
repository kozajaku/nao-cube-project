package org.cvut.kozajaku.dicerecog;

import java.io.File;
import org.cvut.haurvojt.dicerecog.core.Recognizer;

/**
 *
 * @author radio.koza
 */
public class Main {
    public static void main(String[] args) {
        File image = new File("dark2.jpg");
        int res = Recognizer.recognize(image);
        System.out.println("Output: " + res);
    }
}
