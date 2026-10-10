require "json"
require "date"
def add_planner(planner)

  planner_hash =  { task: nil, date: nil, status: nil, shown: true}
  puts("Введіть завдання")
  planner_hash[:task] = gets.chomp
  new_date = nil
  while new_date == nil
    puts("Введіть термін виконання у форматі ДД-ММ-РРРР")
    input_date = gets.chomp
    new_date = valid_date_check(input_date)
    if new_date == nil
    puts("Неправильний формат")
    end
  end
  planner_hash[:date] = new_date
  loop do
  puts("Оберість статус завдання:\n1. Активно\n2. Виконано")
  input_status = gets.chomp
    if input_status == "1"
      planner_hash[:status] = true
      break
    elsif input_status == "2"
      planner_hash[:status] = false
      break
    else
      puts("Такої задачі не існує")
    end
  end
  planner_hash[:shown] = true
  planner << planner_hash
  planner
end
def delete_planner(planner)
  puts("Введіть номер задачі яку хочете прибрати")
  print_planner(planner)
  input_delete = gets.chomp.to_i-1
  if planner[input_delete] == nil || planner[input_delete][:shown] == false
    puts("Такої задачі не існує")
  else
    planner.delete_at(input_delete)
  end
  planner
end

def edit_planner(planner)
  puts("Введіть номер задачі яку хочете змінити")
  print_planner(planner)
  input_edit = gets.chomp.to_i - 1
  if planner[input_edit] == nil || planner[input_edit][:shown] == false
    puts("Такої задачі не існує")
  else
    puts("Що хочете змінити?\n1. Умову завдання\n2. Термін\n3. Статус")
    menu_input_edit = gets.chomp
    if menu_input_edit == "1"
      puts("Введіть нову умову")
      planner[input_edit][:task] = gets.chomp
    elsif menu_input_edit == "2"
      puts("Введіть новий термін виконання у форматі ДД-ММ-РРРР")
      input_date = gets.chomp
      new_date = valid_date_check(input_date)
      if new_date == nil
        puts("Неправильний формат")
      end
      planner[input_edit][:date] = new_date
    elsif menu_input_edit == "3"
      puts("Оберіть новий статус:\n1. Активно\n2. Виконано")
      input_status = gets.chomp
      if input_status == "1"
        planner[input_edit][:status] = true
      elsif input_status == "2"
        planner[input_edit][:status] = false
      end
    else
      puts("Такого варіанту не існує")
    end
  end
  planner
end

def print_planner(planner)
  for i in 0...planner.size
    if planner[i][:shown]
      print("#{i + 1}. #{planner[i][:task]}, #{planner[i][:date]}, ")          #принт чтоб не прееносить строку
      if planner[i][:status]
        puts("Активна")
      else
        puts("Виконана")
      end
    end
  end
end

def filter_planner(planner)
  today = Date.today
  menu1 = true
  puts("Оберіть фільтрацію:\n1. За статусом\n2. За датою\n3. Показати все")
  input_filter = gets.chomp
  if input_filter == "1"
    puts("1. Тільки активні\n2. Тільки виконане")
    menu1_input_filter = gets.chomp
    if menu1_input_filter == "1"
      filter_status = true
    elsif menu1_input_filter == "2"
      filter_status = false
    else
      puts("Такого варіанту не існує")
      menu1 = false
    end
    if menu1
      for i in planner
        if i[:status] != filter_status
          i[:shown] = false
        end
      end
    end
  elsif input_filter == "2"
    puts("1. Задачі сьогодні\n2. Задачі цього місяця\n3. Задачі цього року")
    menu2_input_filter = gets.chomp
    if menu2_input_filter == "1"
      for i in planner
        if i[:date].day != today.day
        i[:shown] = false
        end
      end
    elsif menu2_input_filter == "2"
      for i in planner
        if i[:date].month != today.month
          i[:shown] = false
        end
      end
    elsif menu2_input_filter == "3"
      for i in planner
        if i[:date].year != today.year
          i[:shown] = false
        end
      end
    else
      puts("Такого варіанту не існує")
    end
  elsif input_filter == "3"
    for i in planner
      i[:shown] = true
    end
  else
    puts("Такого варіанту не існує")
  end
  planner
end

def save_planner(planner, file_name)
  File.write(file_name, JSON.generate(planner))
end

def valid_date_check(input_date)
  if input_date =~ /\d{2}-\d{2}-\d{4}/                   # d - число, {} - количество символов
    date_in_format = input_date.split("-")
    for i in 0...date_in_format.size
      date_in_format[i] = date_in_format[i].to_i
    end
    if Date.valid_date?(date_in_format[2],date_in_format[1],date_in_format[0])
      new_date = Date.new(date_in_format[2],date_in_format[1],date_in_format[0])
    else
      new_date = nil
    end
  else
    new_date = nil
  end
  new_date
end
def main_setup(file_name)
  if File.exist?(file_name)
    file = File.read(file_name)
    planner = JSON.parse(file, symbolize_names: true)
    for i in planner
      i[:date] = Date.parse(i[:date])              #парсим как дату потому что она сохраняеться как строка
    end
  else
    puts("Планер не знайдено. Створюю новий...")
    planner = []
  end
  planner
end
def main
  file_name = "planner.json"
  planner = main_setup(file_name)
  loop do
    puts("\n1. Показати планер\n2. Додати задачу\n3. Видалити задачу\n4. Редагувати задачу\n5. Фільтрувати\n\n6. Зберегти і вийти\n")
    input = gets.chomp
    if input == "1"
      print_planner(planner)
    elsif input == "2"
      add_planner(planner)
    elsif input == "3"
      delete_planner(planner)
    elsif input == "4"
      edit_planner(planner)
    elsif input == "5"
      filter_planner(planner)
    elsif input == "6"
      save_planner(planner, file_name)
      exit!
    else
      puts("Такого варіанту не існує")
    end
  end
end

if __FILE__ == $0
  main
end