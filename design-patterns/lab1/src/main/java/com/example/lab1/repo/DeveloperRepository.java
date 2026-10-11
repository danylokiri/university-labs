package com.example.lab1.repo;

import com.example.lab1.models.Developer;
import org.springframework.data.repository.CrudRepository;

public interface DeveloperRepository extends CrudRepository<Developer, Long> {
}
