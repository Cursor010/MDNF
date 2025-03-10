package com.grig.mytraining.ui.home;

import android.annotation.SuppressLint;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import androidx.viewpager2.widget.ViewPager2;

import com.grig.mytraining.R;

import java.util.List;


public class SliderAdapter extends RecyclerView.Adapter<SliderAdapter.SliderViewHolder>{

    private final List<SliderStatItem> sliderItems;
    private final ViewPager2 viewPager2;

    public SliderAdapter(List<SliderStatItem> sliderItems, ViewPager2 viewPager2) {
        this.sliderItems = sliderItems;
        this.viewPager2 = viewPager2;
    }

    @NonNull
    @Override
    public SliderViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new SliderViewHolder(
                LayoutInflater.from(parent.getContext()).inflate(
                        R.layout.home_stat_layout, parent, false
                )
        );
    }

    @Override
    public void onBindViewHolder(@NonNull SliderViewHolder holder, int position) {
        holder.setStat(sliderItems.get(position));
        if (position == sliderItems.size() - 2) {
            viewPager2.post(runnable);
        }
    }

    @Override
    public int getItemCount() {
        return sliderItems.size();
    }

    static class SliderViewHolder extends RecyclerView.ViewHolder {
        private final TextView homeStateTvTitle,homeStatTvContent;

        public SliderViewHolder(@NonNull View itemView) {
            super(itemView);
            homeStateTvTitle = itemView.findViewById(R.id.homeStateTvTitle);
            homeStatTvContent = itemView.findViewById(R.id.homeStatTvContent);
        }
        void setStat(SliderStatItem sliderItem) {
            homeStateTvTitle .setText(sliderItem.getMonth());
            homeStatTvContent.setText("Тренировок: " + sliderItem.getStat());
        }
    }
    private final Runnable runnable = new Runnable() {
        @SuppressLint("NotifyDataSetChanged")
        @Override
        public void run() {
            sliderItems.addAll(sliderItems);
            notifyDataSetChanged();
        }
    };
}
