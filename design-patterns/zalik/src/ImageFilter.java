public interface ImageFilter {
    void apply(byte[] pixels, int width, int height);

    String getName();
}
