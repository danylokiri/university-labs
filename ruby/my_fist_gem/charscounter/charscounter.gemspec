# frozen_string_literal: true

require_relative "lib/charscounter/version"

Gem::Specification.new do |spec|
  spec.name = "charscounter"
  spec.version = Charscounter::VERSION
  spec.authors = ["ddanq4"]
  spec.email = ["120566535+ddanq4@users.noreply.github.com"]
  spec.summary = "Частотний аналіз символів"
  spec.description = "Гем для підрахунку кількогсті входжень кожного символа в рядку."
  spec.required_ruby_version = ">= 3.0.0"


  gemspec = File.basename(__FILE__)
  spec.files = IO.popen(%w[git ls-files -z], chdir: __dir__, err: IO::NULL) do |ls|
    ls.readlines("\x0", chomp: true).reject do |f|
      (f == gemspec) ||
        f.start_with?(*%w[bin/ test/ spec/ features/ .git appveyor Gemfile])
    end
  end
  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

end
