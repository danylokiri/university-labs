import java.io.*;
import java.util.ArrayList;
import java.util.List;

class author implements Serializable {
    private String name;

    public author() {
    }

    public author(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Author{" +
                "name='" + name + '\'' +
                '}';
    }
}

class book implements Serializable {
    private String title;
    private author author;

    public book() {
    }

    public book(String title, author author) {
        this.title = title;
        this.author = author;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public author getAuthor() {
        return author;
    }

    public void setAuthor(author author) {
        this.author = author;
    }

    @Override
    public String toString() {
        return "Book{" +
                "title='" + title + '\'' +
                ", author=" + author +
                '}';
    }
}

class bookshelf implements Serializable {
    private List<book> books;

    public bookshelf() {
        this.books = new ArrayList<>();
    }

    public List<book> getBooks() {
        return books;
    }

    public void setBooks(List<book> books) {
        this.books = books;
    }

    public void addBook(book book) {
        this.books.add(book);
    }

    @Override
    public String toString() {
        return "Bookshelf{" +
                "books=" + books +
                '}';
    }
}

class reader implements Serializable {
    private String name;
    private List<book> borrowedBooks;

    public reader() {
        this.borrowedBooks = new ArrayList<>();
    }

    public reader(String name) {
        this.name = name;
        this.borrowedBooks = new ArrayList<>();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<book> getBorrowedBooks() {
        return borrowedBooks;
    }

    public void setBorrowedBooks(List<book> borrowedBooks) {
        this.borrowedBooks = borrowedBooks;
    }

    public void borrowBook(book book) {
        this.borrowedBooks.add(book);
    }

    @Override
    public String toString() {
        return "Reader{" +
                "name='" + name + '\'' +
                ", borrowedBooks=" + borrowedBooks +
                '}';
    }
}

class library implements Serializable {
    private List<reader> readers;
    private List<bookshelf> bookshelves;

    public library() {
        this.readers = new ArrayList<>();
        this.bookshelves = new ArrayList<>();
    }

    public List<reader> getReaders() {
        return readers;
    }

    public void setReaders(List<reader> readers) {
        this.readers = readers;
    }

    public List<bookshelf> getBookshelves() {
        return bookshelves;
    }

    public void setBookshelves(List<bookshelf> bookshelves) {
        this.bookshelves = bookshelves;
    }

    public void addReader(reader reader) {
        this.readers.add(reader);
    }

    public void addBookshelf(bookshelf bookshelf) {
        this.bookshelves.add(bookshelf);
    }

    @Override
    public String toString() {
        return "Library{" +
                "readers=" + readers +
                ", bookshelves=" + bookshelves +
                '}';
    }
}

public class zavdannya1 {
    public static void main(String[] args) {
        library lib = new library();

        author author1 = new author("Author One");
        author author2 = new author("Author Two");

        book book1 = new book("Book One", author1);
        book book2 = new book("Book Two", author2);

        bookshelf shelf = new bookshelf();
        shelf.addBook(book1);
        shelf.addBook(book2);

        lib.addBookshelf(shelf);

        reader reader1 = new reader("Reader One");
        reader reader2 = new reader("Reader Two");

        reader1.borrowBook(book1);
        reader2.borrowBook(book2);

        lib.addReader(reader1);
        lib.addReader(reader2);

        System.out.println("Current Library State:");
        System.out.println(lib);

        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("library.ser"))) {
            oos.writeObject(lib);
            System.out.println("Library has been serialized.");
        } catch (IOException e) {
            e.printStackTrace();
        }

        library deserializedLib = null;
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream("library.ser"))) {
            deserializedLib = (library) ois.readObject();
            System.out.println("Library has been deserialized.");
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }

        System.out.println("Deserialized Library State:");
        System.out.println(deserializedLib);
    }
}
