import java.io.*;
import java.util.ArrayList;
import java.util.List;

class author implements Externalizable {
    private String name;

    public author() {}

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
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeUTF(name);
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        name = in.readUTF();
    }

    @Override
    public String toString() {
        return "Author{name='" + name + "'}";
    }
}

class book implements Externalizable {
    private String title;
    private author author;

    public book() {}

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
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeUTF(title);
        out.writeObject(author);
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        title = in.readUTF();
        author = (author) in.readObject();
    }

    @Override
    public String toString() {
        return "Book{title='" + title + "', author=" + author + "}";
    }
}

class bookshelf implements Externalizable {
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
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(books.size());
        for (book b : books) {
            out.writeObject(b);
        }
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int size = in.readInt();
        books = new ArrayList<>(size);
        for (int i = 0; i < size; i++) {
            books.add((book) in.readObject());
        }
    }

    @Override
    public String toString() {
        return "Bookshelf{books=" + books + "}";
    }
}

class reader implements Externalizable {
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
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeUTF(name);
        out.writeInt(borrowedBooks.size());
        for (book b : borrowedBooks) {
            out.writeObject(b);
        }
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        name = in.readUTF();
        int size = in.readInt();
        borrowedBooks = new ArrayList<>(size);
        for (int i = 0; i < size; i++) {
            borrowedBooks.add((book) in.readObject());
        }
    }

    @Override
    public String toString() {
        return "Reader{name='" + name + "', borrowedBooks=" + borrowedBooks + "}";
    }
}

class library implements Externalizable {
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
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(readers.size());
        for (reader r : readers) {
            out.writeObject(r);
        }
        out.writeInt(bookshelves.size());
        for (bookshelf bs : bookshelves) {
            out.writeObject(bs);
        }
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int readersSize = in.readInt();
        readers = new ArrayList<>(readersSize);
        for (int i = 0; i < readersSize; i++) {
            readers.add((reader) in.readObject());
        }
        int bookshelvesSize = in.readInt();
        bookshelves = new ArrayList<>(bookshelvesSize);
        for (int i = 0; i < bookshelvesSize; i++) {
            bookshelves.add((bookshelf) in.readObject());
        }
    }

    @Override
    public String toString() {
        return "Library{readers=" + readers + ", bookshelves=" + bookshelves + "}";
    }
}

public class zavdannya3 {
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

        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("library3.ser"))) {
            oos.writeObject(lib);
            System.out.println("Library has been serialized.");
        } catch (IOException e) {
            e.printStackTrace();
        }

        library deserializedLib = null;
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream("library3.ser"))) {
            deserializedLib = (library) ois.readObject();
            System.out.println("Library has been deserialized.");
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }

        System.out.println("Deserialized Library State:");
        System.out.println(deserializedLib);
    }
}
