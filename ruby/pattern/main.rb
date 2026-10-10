class Car
  def drive
    raise NotImplementedError
  end
end

class Sedan < Car
  def drive
    puts 'Ви керуєте комфортним седаном.'
  end
end

class SUV < Car
  def drive
    puts 'Ви керуєте потужним позашляховиком.'
  end
end

class SportsCar < Car
  def drive
    puts 'Ви керуєте швидким спорткаром!'
  end
end

class Pickup < Car
  def drive
    puts 'Ви керуєте практичним пікапом.'
  end
end

class CarFactory
  def self.create_car(type)
    case type
    when :sedan
      Sedan.new
    when :suv
      SUV.new
    when :sports_car
      SportsCar.new
    when :pickup
      Pickup.new
    else
      raise 'Невідомий тип автомобіля'
    end
  end
end
car = CarFactory.create_car(:sedan)
car.drive

car = CarFactory.create_car(:suv)
car.drive
car = CarFactory.create_car(:sports_car)
car.drive
car = CarFactory.create_car(:pickup)
car.drive