package com.example.lab1.repo;

import com.example.lab1.models.GameDetails;
import org.springframework.data.repository.CrudRepository;

public interface GameDetailsRepository extends CrudRepository<GameDetails, Long> {
}
