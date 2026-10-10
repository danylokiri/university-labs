public class BlackWhiteFilter implements ImageFilter {

    @Override
    public void apply(byte[] pixels, int width, int height) {
        for (int i = 0; i < pixels.length; i++) {
            int v = pixels[i] & 0xFF; // 0..255
            pixels[i] = (v < 128) ? (byte) 0 : (byte) 255;
        }
        System.out.println("Applied: " + getName());
    }

    @Override
    public String getName() {
        return "BlackWhiteFilter";
    }
}
