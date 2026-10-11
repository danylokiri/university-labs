package com.example.lab1.controllers;

import com.example.lab1.repo.GameRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class GameController {

    @Autowired
    private GameRepository gameRepository;

    @GetMapping("/games")
    public String gameMain(Model model) {
        model.addAttribute("games", gameRepository.findAll());
        return "game-main";
    }
}
