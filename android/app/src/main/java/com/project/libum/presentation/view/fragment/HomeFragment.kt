package com.project.libum.presentation.view.fragment

import android.annotation.SuppressLint
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.GestureDetector
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.fragment.app.viewModels
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import com.project.libum.R
import com.project.libum.core.utils.SwipeGestureListener
import com.project.libum.data.dto.Book
import com.project.libum.databinding.FragmentHomeBinding
import com.project.libum.presentation.adapter.BookAdapter
import com.project.libum.presentation.adapter.SpacingItemDecoration
import com.project.libum.presentation.view.activity.BookReaderActivity
import com.project.libum.presentation.view.activity.BookReaderActivity.Companion.BOOK_DATA
import com.project.libum.presentation.view.custom.BookView
import com.project.libum.presentation.view.extension.showErrorMessage
import com.project.libum.presentation.viewmodel.HomeViewModel
import com.project.libum.presentation.viewmodel.MainActivityModel
import dagger.hilt.android.AndroidEntryPoint
import kotlinx.coroutines.launch

@AndroidEntryPoint
class HomeFragment : Fragment() {

    private lateinit var binding: FragmentHomeBinding
    private val mainActivityModel: MainActivityModel by activityViewModels<MainActivityModel>()
    private val homeViewModel: HomeViewModel by viewModels()
    private lateinit var bookAdapter: BookAdapter
    private var isActivatedBookStyleButton = false

    private lateinit var gestureDetector: GestureDetector

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        binding = FragmentHomeBinding.inflate(inflater, container, false)
        return binding.root
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.actionField.listStyleChangerButton.setOnClickListener{
            changeStateOfBookStyleButton()
            homeViewModel.changeBookStyleByActivated(isActivatedBookStyleButton)
        }

        mainActivityModel.books.observe(viewLifecycleOwner) { books ->
            bookAdapter.setBooks(books)
        }

        homeViewModel.bookStyle.observe(viewLifecycleOwner){ displayStyle ->
            when(displayStyle){
                BookView.BookDisplayStyle.SLIM -> setSlimBookAdapter()
                BookView.BookDisplayStyle.WIDE -> setWideBookAdapter()
                else -> setWideBookAdapter()
            }
            bookAdapter.setStyle(displayStyle)
        }

        homeViewModel.catalogState.observe(viewLifecycleOwner){
            val (previous, current, next) = homeViewModel.getSurroundingCatalogStates()

            binding.bookCategories.previousCatalogState.text = previous.name
            binding.bookCategories.currentCatalogState.text = current.name
            binding.bookCategories.nextCatalogState.text = next.name
        }

        binding.bookCategories.nextCatalogState.setOnClickListener{
            homeViewModel.changeNextCatalogState()
        }

        binding.bookCategories.previousCatalogState.setOnClickListener{
            homeViewModel.changePreviousCatalogState()
        }

        gestureDetector = GestureDetector(context, SwipeGestureListener(
            onSwipeRight = {
                homeViewModel.changeNextCatalogState()
                Log.d("HomeFragment", "onViewCreated: swipe right")
                           },
            onSwipeLeft = {
                homeViewModel.changePreviousCatalogState()
                Log.d("HomeFragment", "onViewCreated: swipe left")
            }
            )
        )

        binding.catalogBackground.setOnTouchListener { _, event ->
            gestureDetector.onTouchEvent(event)
        }


        initializeBookAdapter()
    }

    private fun initializeBookAdapter(){
        bookAdapter = BookAdapter()
        val spacingDecoration = SpacingItemDecoration(resources.getDimensionPixelSize(R.dimen.item_spacing))
        binding.bookList.addItemDecoration(spacingDecoration)

        bookAdapter.setOnFavoriteClickListener { book, isFavorite ->
            lifecycleScope.launch {
                val result = if (isFavorite) {
                    mainActivityModel.addBookToFavorites(book)
                } else {
                    mainActivityModel.deleteBookToFavorites(book)
                }
                result.onSuccess {
                    bookAdapter.notifyItemChanged(mainActivityModel.books.value!!.indexOf(book))
                }.onFailure {
                    showErrorMessage(context, "Failed to update favorite status for ${book.title}")
                }
            }
        }

        bookAdapter.setOnBookClickView { book: Book ->
            openBookReaderActivity(book)
        }

        binding.bookList.adapter = bookAdapter
    }

    private fun openBookReaderActivity(book: Book){
        val intent = Intent(context, BookReaderActivity::class.java)
        intent.putExtra(BOOK_DATA, book)
        startActivity(intent)
    }

    private fun changeStateOfBookStyleButton(){
        val button = binding.actionField.listStyleChangerButton
        button.isActivated = !button.isActivated
        isActivatedBookStyleButton = button.isActivated
    }

    private fun setWideBookAdapter(){
        binding.bookList.layoutManager = LinearLayoutManager(context)
    }

    private fun setSlimBookAdapter(){
        val gridLayoutManager = GridLayoutManager(context, SLIM_BOOK_IN_ROW_COUNT)
        binding.bookList.layoutManager = gridLayoutManager
    }

    companion object{
        const val SLIM_BOOK_IN_ROW_COUNT = 3
    }
}