package com.example.lab1.repo;

import com.example.lab1.models.Game;
import org.springframework.data.repository.CrudRepository;

public interface GameRepository extends CrudRepository<Game, Long> {
}
