def main
  loop do
    input = gets.chomp
    if input == "exit"
      break
    end
    input_edit = input.gsub(" ","")
    puts input_edit
    end
end

main