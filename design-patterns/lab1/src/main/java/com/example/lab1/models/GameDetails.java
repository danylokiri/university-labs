package com.example.lab1.models;
import jakarta.persistence.*;

@Entity
public class GameDetails {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(columnDefinition = "TEXT")
    private String description;

    private String ageRating;
    private Double price;

    @OneToOne
    @JoinColumn(name = "game_id", unique = true)
    private Game game;

    public GameDetails() {
    }
    public GameDetails(Long id, String description, String ageRating, Double price, Game game) {
        this.id = id;
        this.description = description;
        this.ageRating = ageRating;
        this.price = price;
        this.game = game;
    }
    public Long getId() {
        return id;
    }
    public void setId(Long id) {
        this.id = id;
    }
    public String getDescription() {
        return description;
    }
    public void setDescription(String description) {
        this.description = description;
    }
    public String getAgeRating() {
        return ageRating;
    }
    public void setAgeRating(String ageRating) {
        this.ageRating = ageRating;
    }
    public Double getPrice() {
        return price;
    }
    public void setPrice(Double price) {
        this.price = price;
    }
    public Game getGame() {
        return game;
    }
    public void setGame(Game game) {
        this.game = game;
    }
    @Override
    public String toString() {
        return "GameDetails{" +
                "id=" + id +
                ", ageRating='" + ageRating + '\'' +
                ", price=" + price +
                '}';
    }
}
