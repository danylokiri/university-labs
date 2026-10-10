class Catalog
  def initialize
    @products = []
  end
  def add(item)
    @products.push(item)
  end
  def show_products
    puts @products
  end
end

def main
  catalog = Catalog.new
  loop do
    puts("1 - Показати список\n2 - Додати товар\n3 - Вихід")
    input = gets.chomp
    if input == "1"
      catalog.show_products()
    elsif input == "2"
      item = gets.chomp
      catalog.add(item)
    elsif input == "3"
      break
    end
  end
end

main