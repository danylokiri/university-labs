#1 = Папір
#2 = Камінь
#3 = Ножиці

def winner_is(player_choice, pc_choice)
  if player_choice == pc_choice
    puts "Нічия :/\n\n"
  elsif player_choice > pc_choice || player_choice == 1 && pc_choice == 3
    puts "Ви програли :(\n\n"
  else
    puts "Перемога! :)\n\n"
  end
end

def main_menu
    puts "Оберіть варіант:\n1. Папір\n2. Камінь\n3. Ножиці\n\n4.Назад\n"
    player_choice = gets.chomp.to_i
    if player_choice < 1 || player_choice > 4
      puts "Такого варіанту не існує\n"
      return
    elsif player_choice == 4
      return
    end
    pc_choice = rand(1..3)
    if pc_choice == 1
      puts "Суперник обирає ... Папір!\n"
    elsif pc_choice == 2
      puts "Суперник обирає ... Камінь!\n"
    else
      puts "Суперник обирає ... Ножиці!\n"
    end
    winner_is(player_choice, pc_choice)
end

loop do
  puts "1. Нова гра\n2. Вихід\n"
  pre_menu = gets.chomp.to_i
  if pre_menu == 1
    main_menu
  elsif pre_menu == 2
    puts "Дякую за гру! Приємного відпочінку :)"
    break
  else puts "Такого варіанту не існує\n"
  end
end
