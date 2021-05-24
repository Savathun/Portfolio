using Printf
using SparseArrays
"""
    Name: Daniel Schlatter
    Date: 8/1/19
    Class: CSCI 3415
    Description: Program to take a txt file of Marvel Characters and some data,
    create a collaboration network, and calculate each characters spider man
    number. Then, allow user to enter the number of a character to view their
    Spider Man number.
"""


"""
    read_network(pathname)

Read the Marvel universe network from the file. The format of the file is
given in the referenced papers.
"""
function read_network(pathname)
    # Reads the ith vertex from file
    function read_vertex(i, file)
        m = match(r"""^([0-9]*)\s*"(.*)"$""", readline(file))
        if parse(Int, m[1]) != i
            error("Vertex number $i does not match expected number $line[1]")
        end
        return m[2]
    end
    # Process the input file
    open(pathname) do file
        # Read the *Vertices line
        parsed = split(readline(file))
        if parsed[1] != "*Vertices"
            error("Missing *Vertices line")
        end
        nvertices = parse(Int, parsed[2])
        ncharacters = parse(Int, parsed[3])
        ncomics = nvertices - ncharacters
        # Read vertices - characters and comics
        characters = [read_vertex(i, file) for i = 1:ncharacters]
        comics = [read_vertex(i, file) for i = ncharacters+1:nvertices]
        # Read *Edgeslist line
        if readline(file) != "*Edgeslist" then
            error("Missing *Edgeslist line")
        end
        # Read the edges - appearances
        appearances = spzeros(Int, ncharacters, ncomics)
        while !eof(file)
            parsed = split(readline(file))
            character = parse(Int, parsed[1])
            for i = 2:length(parsed)
                comic = parse(Int, parsed[i]) - ncharacters
                appearances[character, comic] = 1
            end
        end
        return characters, comics, appearances
    end
end

"""
    spidey_numbers(collaborations)

Calculate the Spider Man Number of each Marvel character.
Return array of these numbers.
"""
const SPIDEY = 5306

function spidey_numbers(collaborations)
    println("Calculating Spidey Numbers")
    ncharacters = size(collaborations, 1)
    # Create an array of the spider man numbers
    nums = fill(-1, ncharacters)
    # Create essentially a boolean matrix
    C = collaborations^0
    # fill nums with spider man numbers, doing all 1s first, then 2s, etc.
    for i = 0:6
        for j = 1:ncharacters
            if nums[j] == -1 && C[SPIDEY, j] > 0
                nums[j] = i
            end
        end
        C *= collaborations
    end
    return nums
end

"""
    console_program(characters, nums)

Take number from user, display character name and Spider man number of
that character. Loop until user exits with -9.
"""
function console_program(characters, nums)
    println("Enter the numbers (1 - 6486) of marvel characters (one at a time)")
    i = 0
    # infinite loop
    while(i < 1)
        @label invalid_input
        try
            # Get user input
            n = parse(Int, readline())
            if n == -9
                @goto exit_loop
            end
            str = @sprintf("\"%-s\"", characters[n])
            @printf("%4d %-22s : %-s\n", n, str, nums[n] == -1 ? ">6" : nums[n])
        catch
            # Error check for out of range numbers and wrong types.
            println("Invalid Input. Enter a number between 1 and 6486. Enter -9 to escape")
            @goto invalid_input
        end
    end
    @label exit_loop
    println("Program Exited.")
end
"""
The main program for the Marvel universe assignment. In this hint version it
reads the Marvel universe network from the file "porgat.txt" and prints some
simple statistics to make sure the file was properly read. Then it computes
the collaboration matrix.
"""
function main()
    # Read the network
    println("Reading Marvel universe network")
    characters, comics, appearances = read_network("porgat.txt")
    #ncharacters = length(characters)
    #ncomics = length(comics)
    # Print some statistics
    #println("Number of characters = $ncharacters")
    #println("Number of comics = $ncomics")
    #nappearances = sum(appearances)
    #@printf("Mean books per character = %0.2f\n", nappearances / ncharacters)
    #@printf("Mean characters per book = %0.2f\n", nappearances / ncomics)
    # Compute collaboration matrix
    collaborations = appearances * appearances'
    console_program(characters, spidey_numbers(collaborations))
end

main()
