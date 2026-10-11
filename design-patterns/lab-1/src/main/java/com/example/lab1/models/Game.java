package com.example.lab1.models;

import jakarta.persistence.*;

@Entity
public class Game {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String title;
    private Integer releaseYear;
    private Double rating;

    @ManyToOne
    @JoinColumn(name = "developer_id")
    private Developer developer;

    @ManyToOne
    @JoinColumn(name = "genre_id")
    private Genre genre;

    @OneToOne(mappedBy = "game", cascade = CascadeType.ALL)
    private GameDetails details;

    public Game() {
    }
    public Game(Long id, String title, Integer releaseYear, Double rating,
                Developer developer, Genre genre, GameDetails details) {
        this.id = id;
        this.title = title;
        this.releaseYear = releaseYear;
        this.rating = rating;
        this.developer = developer;
        this.genre = genre;
        this.details = details;
    }
    public Long getId() {
        return id;
    }
    public void setId(Long id) {
        this.id = id;
    }
    public String getTitle() {
        return title;
    }
    public void setTitle(String title) {
        this.title = title;
    }
    public Integer getReleaseYear() {
        return releaseYear;
    }
    public void setReleaseYear(Integer releaseYear) {
        this.releaseYear = releaseYear;
    }
    public Double getRating() {
        return rating;
    }
    public void setRating(Double rating) {
        this.rating = rating;
    }
    public Developer getDeveloper() {
        return developer;
    }
    public void setDeveloper(Developer developer) {
        this.developer = developer;
    }
    public Genre getGenre() {
        return genre;
    }
    public void setGenre(Genre genre) {
        this.genre = genre;
    }
    public GameDetails getDetails() {
        return details;
    }
    public void setDetails(GameDetails details) {
        this.details = details;
        if (details != null) {
            details.setGame(this); // поддержать связь 1–1
        }
    }
    @Override
    public String toString() {
        return "Game{" +
                "id=" + id +
                ", title='" + title + '\'' +
                ", releaseYear=" + releaseYear +
                ", rating=" + rating +
                '}';
    }
}
