require "minitest/autorun"
require "date"
require "json"
require_relative "./planner"

class PlannerTest < Minitest::Test
  def setup
    @planner = []
    @temp_file = "test_planner.json"
  end

  def teardown
    if File.exist?(@temp_file)
      File.delete(@temp_file)
    end
  end

  def test_add_planner_valid
    $stdin = StringIO.new("Test Task\n31-10-2024\n1\n")
    add_planner(@planner)
    $stdin = STDIN
    assert_equal 1, @planner.size
    assert_equal "Test Task", @planner[0][:task]
    assert_equal Date.new(2024, 10, 31), @planner[0][:date]
    assert_equal true, @planner[0][:status]
  end

  def test_add_planner_invalid_date
    $stdin = StringIO.new("Test Task\n32-13-2024\n31-10-2024\n1\n")
    add_planner(@planner)
    $stdin = STDIN
    assert_equal 1, @planner.size
    assert_equal Date.new(2024, 10, 31), @planner[0][:date]
  end

  def test_delete_planner_valid_index
    @planner << { task: "Delete Task", date: Date.new(2024, 10, 31), status: true, shown: true }
    $stdin = StringIO.new("1\n")
    delete_planner(@planner)
    $stdin = STDIN
    assert_equal 0, @planner.size
  end

  def test_delete_planner_invalid_index
    @planner << { task: "Task 1", date: Date.new(2024, 10, 31), status: true, shown: true }
    $stdin = StringIO.new("5\n")
    delete_planner(@planner)
    $stdin = STDIN
    assert_equal 1, @planner.size
  end

  def test_edit_planner_change_task
    @planner << { task: "Old Task", date: Date.new(2024, 10, 31), status: true, shown: true }
    $stdin = StringIO.new("1\n1\nNew Task\n")
    edit_planner(@planner)
    $stdin = STDIN
    assert_equal "New Task", @planner[0][:task]
  end

  def test_edit_planner_change_date
    @planner << { task: "Old Task", date: Date.new(2024, 10, 31), status: true, shown: true }
    $stdin = StringIO.new("1\n2\n15-11-2024\n")
    edit_planner(@planner)
    $stdin = STDIN
    assert_equal Date.new(2024, 11, 15), @planner[0][:date]
  end

  def test_edit_planner_change_status
    @planner << { task: "Old Task", date: Date.new(2024, 10, 31), status: true, shown: true }
    $stdin = StringIO.new("1\n3\n2\n")
    edit_planner(@planner)
    $stdin = STDIN
    assert_equal false, @planner[0][:status]
  end

  def test_print_planner
    @planner << { task: "Test Task", date: Date.new(2024, 10, 31), status: true, shown: true }
    $stdout = StringIO.new
    print_planner(@planner)
    output = $stdout.string
    $stdout = STDOUT
    assert_includes output, "1. Test Task, 2024-10-31, Активна"
  end

  def test_filter_planner_status_active
    @planner = [
      { task: "Active Task", date: Date.today, status: true, shown: true },
      { task: "Completed Task", date: Date.today, status: false, shown: true }
    ]
    $stdin = StringIO.new("1\n1\n")
    filter_planner(@planner)
    $stdin = STDIN
    assert_equal true, @planner[0][:shown]
    assert_equal false, @planner[1][:shown]
  end

  def test_filter_planner_status_completed
    @planner = [
      { task: "Active Task", date: Date.today, status: true, shown: true },
      { task: "Completed Task", date: Date.today, status: false, shown: true }
    ]
    $stdin = StringIO.new("1\n2\n")
    filter_planner(@planner)
    $stdin = STDIN
    assert_equal false, @planner[0][:shown]
    assert_equal true, @planner[1][:shown]
  end

  def test_filter_planner_by_day
    today = Date.today
    @planner = [
      { task: "Today's Task", date: today, status: true, shown: true },
      { task: "Yesterday's Task", date: today - 1, status: true, shown: true }
    ]
    $stdin = StringIO.new("2\n1\n")
    filter_planner(@planner)
    $stdin = STDIN
    assert_equal true, @planner[0][:shown]
    assert_equal false, @planner[1][:shown]
  end

  def test_filter_planner_by_month
    today = Date.today
    @planner = [
      { task: "This Month Task", date: today, status: true, shown: true },
      { task: "Last Month Task", date: today.prev_month, status: true, shown: true }
    ]
    $stdin = StringIO.new("2\n2\n")
    filter_planner(@planner)
    $stdin = STDIN
    assert_equal true, @planner[0][:shown]
    assert_equal false, @planner[1][:shown]
  end

  def test_filter_planner_by_year
    this_year = Date.today.year
    @planner = [
      { task: "This Year Task", date: Date.new(this_year, 6, 15), status: true, shown: true },
      { task: "Last Year Task", date: Date.new(this_year - 1, 12, 31), status: true, shown: true }
    ]
    $stdin = StringIO.new("2\n3\n")
    filter_planner(@planner)
    $stdin = STDIN
    assert_equal true, @planner[0][:shown]
    assert_equal false, @planner[1][:shown]
  end

  def test_save_planner
    @planner << { task: "Save Test", date: Date.new(2024, 12, 25), status: true, shown: true }
    save_planner(@planner, @temp_file)
    assert File.exist?(@temp_file)
    saved_data = JSON.parse(File.read(@temp_file), symbolize_names: true)
    assert_equal "Save Test", saved_data[0][:task]
  end

  def test_main_setup_creates_new_planner_if_file_not_exist
    if File.exist?(@temp_file)
      File.delete(@temp_file)
    end
    $stdout = StringIO.new
    planner = main_setup(@temp_file)
    output = $stdout.string
    $stdout = STDOUT
    assert_equal [], planner
    assert_includes output, "Планер не знайдено. Створюю новий..."
  end

  def test_main_setup_loads_existing_planner
    File.write(@temp_file, JSON.generate([{ task: "Load Test", date: "2024-11-30", status: false, shown: true }]))
    planner = main_setup(@temp_file)
    assert_equal 1, planner.size
    assert_equal "Load Test", planner[0][:task]
    assert_equal Date.new(2024, 11, 30), planner[0][:date]
    assert_equal false, planner[0][:status]
  end
end
